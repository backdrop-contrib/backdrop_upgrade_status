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
    t('This information should prove useful as you are evaluating an upgrade to Backdrop CMS for this site. This report should contain an overview for most of the content on this site.'),
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
  $collapsed = TRUE;
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
    $todo_default['profile'] = 'profile';
  }
  else {
    $collapsed = FALSE;
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
    'title' => t('Profile / Distribution'),
    'info' => t('This is the profile that was used when the site was first installed.'),
    'header' => array(t('Profile name'), t('Enabled'), t('Distribution'), t('Backdrop status'), t('Recommendation')),
    'rows' => $profile_rows,
    'collapsed' => $collapsed,
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

  $collapsed = TRUE;
  $core_modules = array();
  $core_module_rows = array();
  $contrib_modules = array();
  $contrib_module_rows = array();
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
            $collapsed = FALSE;
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
            'drupal' => t('Custom?'),
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

  if ($collapsed) {
    $todo_default['core'] = 'core';
  }
  $core_module_data = array(
    'title' => t('Enabled Core Modules'),
    'info' => t('This is a summary of enabled core modules.'),
    'header' => array(t('Module name'), t('Drupal status'), t('Backdrop status'), t('Recommendation')),
    'rows' => $core_module_rows,
    'collapsed' => $collapsed,
  );
  $renderable['core_modules'] = backdrop_upgrade_status_report_data($core_module_data);


  $contrib_module_data = array(
    'title' => t('Enabled Contrib Modules'),
    'info' => t('This is a summary of enabled contributed modules.'),
    'header' => array(t('Module name'), t('Drupal version'), t('Backdrop status'), t('Recommendation')),
    'collapsed' => FALSE,
  );
  
  if ($available) {
    $contrib_module_data['rows'] = $contrib_module_rows;
    $contrib_module_data['collapsed'] = $collapsed;
  }
  else {
    $run_cron = t('Run cron');
    if (user_access('administer site configuration')) {
      $cron_url = url('admin/reports/status/run-cron');
      $run_cron = '<a href="' . $cron_url . '">' . $run_cron . '</a>';
    }
    
    $replacements = array('!cron' => $run_cron);
    $run_cron = t('!cron to retrieve contributed module status.', $replacements);
    $replacements = array('!run_cron' => $run_cron);
    $help = '<p>' . t('<strong>No contrib data available yet.</strong> !run_cron', $replacements) . '</p>';
    $contrib_module_data['rows'] = array(array(array('data' => $help, 'colspan' => 4)));
  }
  $renderable['contrib_modules'] = backdrop_upgrade_status_report_data($contrib_module_data);
  
  $custom_module_data = array(
    'title' => t('Enabled Custom Modules'),
    'info' => t('This is a summary of enabled custom modules.'),
    'header' => array(t('Module name'), t('Drupal status'), t('Backdrop status'), t('Recommendation')),
    'rows' => $custom_module_rows,
    'collapsed' => FALSE,
  );
  $renderable['custom_modules'] = backdrop_upgrade_status_report_data($custom_module_data);



  // Updtates defaults based on changed info from above.
  $renderable['intro']['backdrop_upgrade_status_todo']['#default_value'] = $todo_default;


  // Add fieldsets for all modules that provide one.
  foreach (module_implements('backdrop_upgrade_status_report') as $module) {
    $module_data = module_invoke($module, 'backdrop_upgrade_status_report');

    $machine = $module_data['machine'];
    unset($module_data['machine']);

    $todo_status = $module_data['todo_status'];
    unset($module_data['todo_status']);

    // Set default value for checkbox.
    $renderable['intro']['backdrop_upgrade_status_todo']['#default_value'][$machine] = $todo_status;
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
 *
 * @return array
 *   Renderable fieldset.
 */
function backdrop_upgrade_status_report_data($variables) {
  $title = $variables['title'];
  $info = $variables['info'];
  $header = $variables['header'];
  $rows = $variables['rows'];
  $collapsed = isset($variables['collapsed'])? $variables['collapsed']: TRUE;

  if (!empty($rows)) {
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
      )),
    );
  }

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

  // Set the starting default value based on whats in the DataBase.
  $todo_default = variable_get('backdrop_upgrade_status_todo', array());

  // Here's the list of possible overvoew options. Let's make this pluggable?
  $todo_items = array(
    'profile' => array(
      'review' => 'system profile',
      'description' => 'Check that a Backdrop version is available.',
      'weight' => -10,
    ),
    'core_modules' => array(
      'review' => 'core modules',
      'description' => 'Disable and uninstall anthing not in use.',
      'weight' => -10,
    ),
    'contrib_modules' => array(
      'review' => 'contrib modules',
      'description' => 'Disable and uninstall anthing not in use, or nolonger necessary.',
      'weight' => -10,
    ),
    'custom_modules' => array(
      'review' => 'custom modules',
      'description' => 'Disable and uninstall anthing not in use, or nolonger necessary.',
      'weight' => -10,
    ),
    'theme' => array(
      'description' => 'Swicth to the Bartik theme for the upgrade (Useful if you have not yet ported your Drupal 7 theme to Backdrop).',
      'optional' => TRUE,
      'weight' => 0,
    ),
  );

  // Add checkboxes for all modules that provide one.
  foreach (module_implements('backdrop_upgrade_status_todo') as $module) {
    $module_todos = module_invoke($module, 'backdrop_upgrade_status_todo');
    foreach ($module_todos as $machine => $item) {
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

  $form['intro']['backdrop_upgrade_status_todo'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Prepare for upgrading to Backdrop CMS:'),
    '#options' => $todo_options,
    '#default_value' => $todo_default,
  );

  $form['intro']['save'] = array(
    '#type' => 'button',
    '#value' => t('Save Progress'),
    '#submit' => array('backdrop_upgrade_status_report_checklist_form_submit'),
  );
  $form['intro']['scan'] = array(
    '#type' => 'button',
    '#value' => t('Scan this site'),
    '#submit' => array('backdrop_upgrade_status_report_checklist_form_scan'),
    '#disabled' => TRUE,
  );

  return $form;
}

/**
 * Submit handler for backdrop_upgrade_status_report_checklist_form().
 */
function backdrop_upgrade_status_report_checklist_form_submit($form, &$form_state) {
  if (isset($form_state['values']['backdrop_upgrade_status_todo'])) {
    $todo = $form_state['values']['backdrop_upgrade_status_todo'];
    variable_set('backdrop_upgrade_status_todo', $todo);
  }
}

/**
 * Submit handler for backdrop_upgrade_status_report_checklist_form().
 */
function backdrop_upgrade_status_report_checklist_form_scan() {
  drupal_set_message('Not working just yet.');
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
      'rec' => t('Add the contrib version of Aggregtor to Backdrop.'),
    ),
    'blog' => array(
      'class' => 'error',
      'rec' => t('Delete content, Convert to custom content type, or port to Backdrop.'),
    ),
    'dashboard' => array(
      'class' => '',
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
