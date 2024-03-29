<?php
/**
 * @file
 * Backdrop upgrade Overview Report.
 */

/**
 * Generates an informational report showing everything you'll need to upgrade
 * your site from Drupal to Backdrop CMS.
 *
 * @return array
 *   Renderable tables with all the info!
 */
function backdrop_upgrade_status_report() {
  $welcome = array(
    t('This information should prove useful as you are evaluating an upgrade to Backdrop CMS for this site.'),
  );
  $renderable['welcome'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . implode('</p><p>', $welcome) . '</p>',
  );

  // Add the checklist as a separate form.
  $renderable = backdrop_upgrade_status_report_checklist_form($renderable);

  // Add the profile data.
  $profile = drupal_get_profile();
  $info = system_get_info('module', $profile);
  // Get current state. Defaults to FALSE / open.
  $profile_collapsed = backdrop_upgrade_status_get_state('profile');
  $recommendation = '';
  $backdrop_status = t('Supported');
  $drupal_status = db_query("SELECT status FROM {system} WHERE name = :profile", array(':profile' => $profile))->fetchField();
  if ($drupal_status) {
    $enabled = t('Enabled');
  }
  else {
    $enabled = t('Disabled');
    $class = 'warning';
  }

  if ($info['distribution_name'] == 'Drupal') {
    // @todo, don't collapse when set not reviewed?
    $profile_collapsed = TRUE;
  }
  else {
    $recommendation = t('Check if the distribution is available for Backdrop, or use the <a href="!url">profile switcher</a> module to change to the Standard profile.', array('!url' => 'https://www.drupal.org/project/profile_switcher'));
    $backdrop_status = t('Unknown');
    $class = 'warning';
  }
  $profile_rows = array(
    array(
      'data' => array(
        'name' => $info['name'],
        'status' => $enabled,
        'distro' => $info['distribution_name'],
        'backdrop' => $backdrop_status,
        'rec' => $recommendation,
      ),
      'class' => isset($class) ? array($class) : array(),
    ),
  );

  $profile_data = array(
    'machine' => 'profile',
    'title' => t('Install Profile / Distribution'),
    'info' => t('This is the Install Profile or Distribution that was used when the site was first installed.'),
    'header' => array(t('Profile name'), t('Enabled'), t('Distribution'), t('Backdrop status'), t('Recommendation')),
    'rows' => $profile_rows,
    'collapsed' => $profile_collapsed,
  );
  $renderable['profile'] = backdrop_upgrade_status_report_data($profile_data);


  // Get all module data.
  $projects = system_rebuild_module_data();
  // Get list of core modules removed from Backdrop.
  $removed_modules = _backdrop_upgrade_status_removed_core_modules();

  // Get backdrop project data.
  if ($available = backdrop_upgrade_status_get_available()) {
    module_load_include('inc', 'backdrop_upgrade_status', 'backdrop_upgrade_status.compare');
    $backdrop_data = backdrop_upgrade_status_calculate_project_data($available);
  }

  $core_modules = array();
  $core_module_rows = array();
  $core_collapsed = backdrop_upgrade_status_get_state('core_modules');

  $contrib_modules = array();
  $contrib_module_rows = array();
  $contrib_collapsed = backdrop_upgrade_status_get_state('contrib_modules');

  $last_project = '';
  foreach ($projects as $project) {
    $to_skip = array('views_ui', 'backdrop_upgrade_status');
    if (in_array($project->name, $to_skip)) {
      continue;
    }

    if ($project->type == 'module' && $project->status == 1) {

      // Core modules.
      if ($project->info['package'] == 'Core') {
        $core_modules[$project->name] = $project;
        if (array_key_exists($project->name, $removed_modules)) {
          $backdrop_status = t('Removed from core');

          $recommendation = $removed_modules[$project->name]['rec'];
          if (!empty($removed_modules[$project->name]['class'])) {
            $class = $removed_modules[$project->name]['class'];
            // @todo, don't collapse when set not reviewed?
            $core_collapsed = FALSE;
          }
        }
        else {
          $backdrop_status = t('In core');
          $recommendation = '';
          $class = '';
        }
        $row = array(
          'data' => array(
            'name' => $project->info['name'],
            'drupal' => t('In core'),
            'backdrop' => $backdrop_status,
            'rec' => t($recommendation, array(':module' => $project->info['name'])),
          ),
          'class' => isset($class) ? array($class) : array(),
        );
        $core_module_rows[] = $row;
      }

      // Contrib modules.
      elseif (isset($project->info['version'])) {
        if (isset($project->info['project']) && $project->info['project'] != $last_project) {
          $drupal_version = $project->info['version'];
          $backdrop_version = '';
          $recommendation = '';
          $class = '';
          if ($available && array_key_exists($project->name, $backdrop_data)) {
            switch ($backdrop_data[$project->name]['status']) {
              case BACKDROP_UPGRADE_STATUS_CORE:
                $backdrop_version = '';
                if (isset($backdrop_data[$project->name]['in_core_note'])) {
                  $backdrop_version .= $backdrop_data[$project->name]['in_core_note'] . '<br>';
                }
                if (isset($backdrop_data[$project->name]['in_core_warning'])) {
                  $backdrop_version .= $backdrop_data[$project->name]['in_core_warning'];
                }
                $status = '';
                if (isset($backdrop_data[$project->name]['in_core_upgrade_status'])) {
                  $status = $backdrop_data[$project->name]['in_core_upgrade_status'];
                }
                $rec_translated = '';
                if (isset($backdrop_data[$project->name]['in_core_upgrade'])) {
                  $rec_translated = $backdrop_data[$project->name]['in_core_upgrade'];
                }
                break;
              case BACKDROP_UPGRADE_STATUS_STABLE;
                $backdrop_version = t('Stable :bversion', array(':bversion' => $backdrop_data[$project->name]['latest_version']));
                $status = t('Leave enabled.');
                $recommendation = 'Before upgrade: Add the stable contrib version of :module to Backdrop.';
                break;
              case BACKDROP_UPGRADE_STATUS_DEVELOPMENT:
                $backdrop_version = t('In Development :bversion', array(':bversion' => $backdrop_data[$project->name]['latest_version']));
                $recommendation = 'Before upgrade: Add the development contrib version of :module to Backdrop.';
                $class = 'warning';
                break;
              case UPDATE_UNKNOWN:
                // fall-through intentional.
              default:
                $backdrop_version = t('Unknown');
                $status = t('Leave enabled.');
                $recommendation = 'Port :module to Backdrop.';
                $class = 'error';
                break;
            }
          }

          $note = '';
          if (isset($status)) {
            $note .= '<strong>' . $status . '</strong><br>';
          }
          if (isset($recommendation)) {
            $note .=  t($recommendation, array(':module' => $project->info['name']));
          }
          elseif (isset($rec_translated))  {
            $node .= $rec_translated;
          }

          $row = array(
            'data' => array(
              'name' => $project->info['name'],
              'drupal' => $drupal_version,
              'backdrop' => $backdrop_version,
              'note' => $note,
            ),
            'class' => isset($class) ? array($class) : array(),
          );
          $contrib_module_rows[] = $row;
        }
      }

      // Custom modules.
      else {
        $custom_modules[$project->name] = $project;
        $row = array(
          'data' => array(
            'name' => $project->info['name'],
            'drupal' => t('Custom'),
            'backdrop' => t('Unknown'),
            'rec' => t('Port :module to Backdrop.', array(':module' => $project->info['name'])),
          ),
          'class' => array('error'),
        );
        $custom_module_rows[] = $row;
      }

      // Save the last project so we don't include sub-modules in this list.
      if (isset($project->info['project'])) {
        $last_project = $project->info['project'];
      }
      else {
        $last_project = $project->name;
      }
    }
  }

  $core_module_data = array(
    'title' => t('Enabled Core Modules'),
    'info' => t('This is a summary of enabled core modules.'),
    'header' => array(t('Module name'), t('Drupal status'), t('Backdrop status'), t('Recommendation')),
    'rows' => $core_module_rows,
    'collapsed' => $core_collapsed,
  );
  $renderable['core_modules'] = backdrop_upgrade_status_report_data($core_module_data);

  $contrib_module_data = array(
    'title' => t('Enabled Contrib Modules'),
    'info' => t('This is a summary of enabled contributed modules.'),
    'header' => array(t('Module name'), t('Drupal version'), t('Backdrop status'), t('Recommendation')),
    'empty' => t('No enabled contributed modules have been detected.'),
    'collapsed' => $contrib_collapsed,
  );

  if ($available) {
    $contrib_module_data['rows'] = $contrib_module_rows;
  }
  else {
    $help = _backdrop_upgrade_status_help();
    $contrib_module_data['rows'] = array(array(array('data' => $help, 'colspan' => 4)));
  }
  $renderable['contrib_modules'] = backdrop_upgrade_status_report_data($contrib_module_data);

  // Get current state, don't try to guess.
  $custom_collapsed = backdrop_upgrade_status_get_state('custom_modules');

  $custom_module_data = array(
    'title' => t('Enabled Custom Modules'),
    'info' => t('This is a summary of enabled custom modules.'),
    'header' => array(t('Module name'), t('Drupal status'), t('Backdrop status'), t('Recommendation')),
    'rows' => $custom_module_rows,
    'empty' => t('No enabled custom modules have been detected.'),
    'collapsed' => $custom_collapsed,
  );
  $renderable['custom_modules'] = backdrop_upgrade_status_report_data($custom_module_data);

  // Add fieldsets for all modules that provide one.
  foreach (module_implements('backdrop_upgrade_status_report') as $module) {
    $module_data = module_invoke($module, 'backdrop_upgrade_status_report');

    // Use machine for the fieldset name only.
    $machine = $module_data['machine'];
    unset($module_data['machine']);

    // Add fieldset for module data.
    $renderable[$machine] = backdrop_upgrade_status_report_data($module_data);
  }

  // Allow other modules to add things to the reports page.
  drupal_alter('backdrop_upgrade_status_report', $renderable);

  return system_settings_form($renderable);
}

// Include all the
$path = drupal_get_path('module', 'backdrop_upgrade_status') . '/includes/';
include_once(DRUPAL_ROOT . '/' . $path . 'backdrop_upgrade_status.node.inc');
include_once(DRUPAL_ROOT . '/' . $path . 'backdrop_upgrade_status.field.inc');
include_once(DRUPAL_ROOT . '/' . $path . 'backdrop_upgrade_status.image.inc');
include_once(DRUPAL_ROOT . '/' . $path . 'backdrop_upgrade_status.taxonomy.inc');
include_once(DRUPAL_ROOT . '/' . $path . 'backdrop_upgrade_status.user.inc');
include_once(DRUPAL_ROOT . '/' . $path . 'backdrop_upgrade_status.paragraphs.inc');
include_once(DRUPAL_ROOT . '/' . $path . 'backdrop_upgrade_status.views.inc');
include_once(DRUPAL_ROOT . '/' . $path . 'backdrop_upgrade_status.features.inc');


/**
 * Creates a fieldset with tabular data for the reports page.
 *
 * @param array $variables
 *   Contains the following keys:
 *   - title: The content for fieldset label, as passed through t().
 *   - info: The content for the introductary paragraph, passed through t().
 *   - header: Array of columnn headers for data, each passed through t().
 *   - rows: Table of data (must match column headers), sanitized.
 *   - collapsed: Whether or not the fieldset should be collapsed.
 *   - empty: Text to display when there are no rows.
 *
 * Note: the key 'machine' should not be included in $variables.
 *
 * @return array
 *   Renderable fieldset.
 */
function backdrop_upgrade_status_report_data($variables) {
  $title = $variables['title'];
  $info = $variables['info'];
  $header = $variables['header'];
  $rows = $variables['rows'];
  $collapsed = $variables['collapsed'];
  $empty = $variables['empty'];

  $fieldset = array(
    '#type' => 'fieldset',
    '#title' => $title,
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
  );
  $fieldset['info'] = array(
    '#type' => 'markup',
    '#markup' => $info,
  );
  $fieldset['table'] = array(
    '#type' => 'markup',
    '#markup' => theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('class' => array('backdrop-upgrade-table')),
      'empty' => $empty,
    )),
  );

  return $fieldset;
}

/**
 * Generates an informational report showing everything you'll need to upgrade
 * your site from Drupal to Backdrop CMS.
 *
 * @see backdrop_upgrade_status_report_checklist_form_submit().
 *
 * @return array
 *   Renderable tables with all the info!
 */
function backdrop_upgrade_status_report_checklist_form($form = array()) {

  $form['intro'] = array(
    '#type' => 'fieldset',
    '#title' => t('Backdrop Upgrade to-do list'),
    '#description' => t('When you are ready to perform the upgrade, this list should walk you through the steps necessary to prepare your Drupal 7 database.'),
    '#collapsible' => TRUE,
  );

  // Here's the list of possible overvoew options. Let's make this pluggable?
  $todo_items = array(
    'profile' => array(
      'review' => 'system profile',
      'description' => 'Check that a Backdrop version is available.',
      'weight' => -10,
    ),
    'core_modules' => array(
      'review' => 'core modules',
      'description' => 'Disable and uninstall anything not in use.',
      'weight' => -10,
    ),
    'contrib_modules' => array(
      'review' => 'contrib modules',
      'description' => 'Disable and uninstall anything not in use, or no longer necessary.',
      'weight' => -10,
    ),
    'custom_modules' => array(
      'review' => 'custom modules',
      'description' => 'Disable and uninstall anything not in use, or no longer necessary.',
      'weight' => -10,
    ),
    'bartik' => array(
      'description' => 'Switch to the Bartik theme for the upgrade (Useful if you have not yet ported your Drupal 7 theme to Backdrop).',
      'optional' => TRUE,
      'weight' => 100, // Ensure this comes last.
    ),
  );

  // Add checkboxes for all modules that provide one.
  foreach (module_implements('backdrop_upgrade_status_todo') as $module) {
    $module_todos = module_invoke($module, 'backdrop_upgrade_status_todo');
    foreach ($module_todos as $machine => $item) {
      // Move optional items to the bottom of the list.
      if ($item['optional'] === TRUE) {
        $item['weight'] = $item['weight'] + 10;
      }
      $todo_items[$machine] = $item;
    }
  }
  drupal_alter('backdrop_upgrade_status_todo', $todo_items);
  uasort($todo_items, 'drupal_sort_weight');

  $todo_options = array();
  foreach ($todo_items as $machine => $item) {
    $output = '';
    if (isset($item['optional']) && $item['optional']) {
      $output .= t('(optional)') . ' ';
    }
    if (isset($item['review'])) {
      $id = drupal_clean_css_identifier('#edit-' . $machine);
      $output .= t('Review <a href="#' . $id . '">' . $item['review'] . '</a>:')  . ' ';
    }
    if (isset($item['description'])) {
      $output .= t($item['description']);
    }
    $todo_options[$machine] = $output;
  }

  // Set the starting default value based on whats in the database.
  $todo_status = variable_get('backdrop_upgrade_status_todo', array());

  $form['intro']['backdrop_upgrade_status_todo'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Prepare for upgrading to Backdrop CMS:'),
    '#options' => $todo_options,
    '#default_value' => $todo_status,
  );

  $form['intro']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save Progress'),
    '#attributes' => array('class' => array('button-primary')),
    '#submit' => array('backdrop_upgrade_status_report_checklist_form_submit'),
  );
  $form['intro']['scan'] = array(
    '#type' => 'submit',
    '#value' => t('Scan this site - coming soon'),
    '#submit' => array('backdrop_upgrade_status_report_checklist_form_scan'),
    '#disabled' => TRUE,
  );
  $form['intro']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear progress'),
    '#submit' => array('backdrop_upgrade_status_report_checklist_form_clear'),
  );

  return $form;
}

/**
 * Submit handler for backdrop_upgrade_status_report_checklist_form().
 */
function backdrop_upgrade_status_report_checklist_form_submit($form, &$form_state) {
  if (isset($form_state['values']['backdrop_upgrade_status_todo'])) {
    $save_me = array();
    foreach ($form_state['values']['backdrop_upgrade_status_todo'] as $key => $value) {
      if ($value !== 0) {
        $save_me[] = $key;
      }
    }
    variable_set('backdrop_upgrade_status_todo', $save_me);
  }

  drupal_set_message(t('The checklist has been saved.'));
}

/**
 * Submit handler for backdrop_upgrade_status_report_checklist_form().
 */
function backdrop_upgrade_status_report_checklist_form_scan($form, &$form_state) {
  $todo_status = variable_get('backdrop_upgrade_status_todo', array());

  $checklist_data = array();
  foreach (module_implements('backdrop_upgrade_status_scan') as $module) {
    $checklist_data += module_invoke($module, 'backdrop_upgrade_status_scan');
  }

  // Add additional checks, but do not remove.
  foreach ($checklist_data as $key => $value) {
    if ($value) {
      $todo_status[] = $key;
    }
  }

  variable_set('backdrop_upgrade_status_todo', $todo_status);

  drupal_set_message(t('This site has been scanned. Additional items may have been cheked off if we believe they are ready for upgrade.'));
}

/**
 * Submit handler for backdrop_upgrade_status_report_checklist_form().
 */
function backdrop_upgrade_status_report_checklist_form_clear($form, &$form_state) {
  variable_set('backdrop_upgrade_status_todo', array());

  drupal_set_message(t('The checklist has been cleared.'));
}

/**
 * Get list of core modules removed from Backdrop, and what to do about each.
 *
 * @return array
 *   List of modules removed from core as an array with the follwing keys:
 *   - class: Class to color-code the row of the modules table.
 *   - rec: Recommendation for how to proceed.
 */
function _backdrop_upgrade_status_removed_core_modules() {
  $drupal_url = 'https://drupal.org/project/';
  // @todo - remove t() here and add instead at render point.
  return array(
    'aggregator' => array(
      'name' => '',
      'class' => 'warning',
      'rec' => t('Add the contrib version of Aggregator to Backdrop.'),
    ),
    'blog' => array(
      'class' => 'error',
      'rec' => t('Convert to custom content type, delete content, or port the Blog module to Backdrop.'),
    ),
    'dashboard' => array(
      'class' => 'warning',
      'rec' => t('Alternaive core module provided. No action required.'),
    ),
    'forum' => array(
      'class' => 'warning',
      'rec' => t('Add the contrib version of Forum to Backdrop.'),
    ),
    'help' => array(
      'class' => 'warning', // Warning, because help is rarely necessary.
      'rec' => t('Disable and Uninstall :module, or port to Backdrop.)'),
    ),
    'openid' => array(
      'class' => 'error',
      'rec' => t('Port :module to Backdrop, or Disable and Uninstall.'),
    ),
    'overlay' => array(
      'class' => 'warning', // Warning, because overlay is rarely necessary.
      'rec' => t('Disable and Uninstall :module, or port to Backdrop.'),
    ),
    'php' => array(
      'class' => 'error',
      'rec' => t('Review all PHP code: delete or change format. Disable and Uninstall PHP Filter module.'),
    ),
    'poll' => array(
      'class' => 'error',
      'rec' => t('Port Poll modile to Backdrop, or Disable and Uninstall.'),
    ),
    'profile' => array(
      'class' => 'warning',
      'rec' => t('Add the contrib version of :module to Backdrop.'),
    ),
    'rdf' => array(
      'class' => 'warning', // Warning, because rdf is rarely necessary.
      'rec' => t('Disable and Uninstall :module, or port to Backdrop.'),
    ),
    'shortcut' => array(
      'class' => 'warning', // Warning, because shortcut is rarely necessary.
      'rec' => t('Disable and Uninstall :module, or port to Backdrop.'),
    ),
    'statistics' => array(
      'class' => 'warning', // Warning, because statistics is rarely necessary.
      'rec' => t('Disable and Uninstall :module, or port to Backdrop.'),
    ),
    'toolbar' => array(
      'class' => '',
      'rec' => t('Alternaive core module provided. No action required.'),
    ),
    'tracker' => array(
      'class' => 'warning', // Warning, because tracker is rarely necessary.
      'rec' => t('Disable and Uninstall :module, or port to Backdrop.'),
    ),
    'trigger' => array(
      'class' => 'warning',
      'rec' => t('Add the contrib version of :module to Backdrop.'),
    )
  );
}
