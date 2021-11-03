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
  $renderable = array();

  $welcome = array(
    t('This information should prove useful as you are evaluating an upgrade to Backdrop CMS for this site. This report should contain an overview for most of the content on this site.'),
  );

  $renderable['welcome'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . implode('</p><p>', $welcome) . '</p>',
  );

  $renderable['intro'] = array(
    '#type' => 'fieldset',
    '#title' => t('Backdrop Upgrade to-do list'),
    '#description' => t('When you are ready to perform the upgrade, this list should walk you through the steps necessary to prepare your Drupal 7 database.'),
    '#collapsible' => TRUE,
  );
  // Set the starting default value based on whats in the DataBase.
  $todo_default = variable_get('backdrop_upgrade_status_todo', array());
  $renderable['intro']['backdrop_upgrade_status_todo'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Prepare for upgrading to Backdrop CMS:'),
    '#options' => array(
      'profile' => t('Review <a href="#edit-profile">system profile</a>: check that Backdrop version is available.'),
      'core' => t('Review <a href="#edit-core-modules">core modules</a>: disable and uninstall anthing not in use.'),
      'contrib' => t('Review <a href="#edit-contrib-modules">contrib modules</a>: disable and uninstall anthing not in use.'),
      'custom' => t('Review <a href="#edit-custom-modules">custom modules</a>: disable and uninstall anthing not in use.'),
      'image_styles' => t('Review <a href="#edit-image-styles">image styles</a>: Configuration must be in the database to convert automatically durring upgrade.'),
      'views' => t('Review <a href="#edit-views">views</a>: Configuration must be in the database to convert automatically durring upgrade.'),
      'features' => t('Review <a href="#edit-features">features</a>: Configuration must be in the database to convert automatically durring upgrade.'),
      'theme' => t('(optional) Swicth to the Bartik theme for the upgrade (Useful if you have not yet ported your Drupal 7 theme to Backdrop)'),
      'node' => t('(optional) Review <a href="#edit-nodes">content types</a>: remove any content types that are not wanted anymore.'),
      'field' => t('(optional) Review <a href="#edit-fields">fields</a>: remove fields that are not wanted anymore.'),
      'taxonomy' => t('(optional) Review <a href="#edit-terms">taxonomy</a>: remove terms or vocabularies that are not wanted anymore.'),
      'paragraph' => t('(optional) Review <a href="#edit-paragraphs">paragraphs</a>: remove paragraphs that are not wanted anymore.'),
      'user' => t('(optional) Review <a href="#edit-users">user accounts</a>: remove old or blocked accounts not in use.'),
    ),
    // Defaults will be adjusted again, before the end of the function.
    '#default_value' => $todo_default,
  );
  $renderable['intro']['save'] = array(
    '#type' => 'button',
    '#value' => t('Save Progress'),
    '#disabled' => TRUE,
  );

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
    'info' => t('This is a summary of the enabled core modules.'),
    'header' => array(t('Module name'), t('Drupal status'), t('Backdrop status'), t('Recommendation')),
    'rows' => $core_module_rows,
    'collapsed' => $collapsed,
  );
  $renderable['core_modules'] = backdrop_upgrade_status_report_data($core_module_data);


  if ($available) {
    $contrib_module_data = array(
      'title' => t('Enabled Contrib Modules'),
      'info' => t('This is a summary of the enabled contributed modules.'),
      'header' => array(t('Module name'), t('Drupal version'), t('Backdrop status'), t('Recommendation')),
      'rows' => $contrib_module_rows,
      'collapsed' => $collapsed,
    );
    $renderable['contrib_modules'] = backdrop_upgrade_status_report_data($contrib_module_data);
  }

  $custom_module_data = array(
    'title' => t('Enabled Custom Modules'),
    'info' => t('This is a summary of the enabled custom modules.'),
    'header' => array(t('Module name'), t('Drupal status'), t('Backdrop status'), t('Recommendation')),
    'rows' => $custom_module_rows,
    'collapsed' => FALSE,
  );
  $renderable['custom_modules'] = backdrop_upgrade_status_report_data($custom_module_data);

  // Get all the users in the system.
  $rows = array();
  $statuses = array(1 => t('Active'), 0 => t('Blocked'));
  foreach ($statuses as $status => $label) {
    $count = db_query("SELECT count(uid) FROM {users} WHERE status = :status", array(':status' => $status))->fetchField();
    $row = array(
      'data' => array(
        'type' => $label,
        'machine' => $status,
        'count' => $count,
      ),
    );
    if ($count == 0) {
      $row['class'] = array('warning');
    }
    $rows[] = $row;
  }
  $user_data = array(
    'title' => t('User account data'),
    'info' => t('This is a summary of the user accounts on the site.'),
    'header' => array(t('User accounts'), t('Status'), t('Number of users')),
    'rows' => $rows,
  );
  $renderable['users'] = backdrop_upgrade_status_report_data($user_data);

  // Get all the node types in the system.
  $rows = array();
  $node_types = node_type_get_names();
  foreach ($node_types as $machine => $node_type) {
    $count = db_query("SELECT count(nid) FROM {node} WHERE type = :machine", array(':machine' => $machine))->fetchField();
    $row = array(
      'data' => array(
        'type' => $node_type,
        'machine' => $machine,
        'count' => $count,
        'recommendation' => '',
      ),
    );
    if ($count == 0) {
      $row['class'] = array('warning');
      $row['data']['recommendation'] = t('Delete this content type?');
    }
    $rows[] = $row;
  }
  $node_data = array(
    'title' => t('Content types'),
    'info' => t('Any conent types with zero itesm could be deleted.'),
    'header' => array(t('Conent type'), t('Machine name'), t('Number of items'), t('Recommendation')),
    'rows' => $rows,
  );
  $renderable['nodes'] = backdrop_upgrade_status_report_data($node_data);

  // Get all the taxonomy terms in the system.
  $rows = array();
  $term_types = taxonomy_get_vocabularies();
  foreach ($term_types as $vid => $vocab) {
    $count = db_query("SELECT count(tid) FROM {taxonomy_term_data} WHERE vid = :machine", array(':machine' => $vid))->fetchField();
    $row = array(
      'data' => array(
        'type' => $vocab->name,
        'vid' => $vid,
        'machine' => $vocab->machine_name,
        'count' => $count,
      ),
    );
    if ($count == 0) {
      $row['class'] = array('warning');
    }
    $rows[] = $row;
  }
  $term_data = array(
    'title' => t('Vocabulary & Term data'),
    'info' => t('Any vocabularies with zero terms could be deleted.'),
    'header' => array(t('Vocabulary'), t('Vocabulary ID'), t('Machine name'), t('Number of terms')),
    'rows' => $rows,
  );
  $renderable['terms'] = backdrop_upgrade_status_report_data($term_data);

  // Get all fields in the system.
  $instances = field_info_instances();
  $field_types = field_info_field_types();
  $bundles = field_info_bundles();

  $modules = system_rebuild_module_data();
  $rows = array();
  foreach ($instances as $entity_type => $type_bundles) {
    foreach ($type_bundles as $bundle => $bundle_instances) {
      foreach ($bundle_instances as $field_name => $instance) {
        $field = field_info_field($field_name);

        // Initialize the row if we encounter the field for the first time.
        if (!isset($rows[$field_name])) {
          $rows[$field_name]['class'] = $field['locked'] ? array('menu-disabled') : array('');
          $rows[$field_name]['data']['name'] = $field['locked'] ? t('@field_name (Locked)', array('@field_name' => $field_name)) : $field_name;
          $rows[$field_name]['data']['label'] = $field_types[$field['type']]['label'];
          $module_name = $field_types[$field['type']]['module'];
          $rows[$field_name]['data']['module'] = $modules[$module_name]->info['name'];
        }

        // Add the current instance.
        $admin_path = _field_ui_bundle_admin_path($entity_type, $bundle);
        $rows[$field_name]['data']['bundles'][] = $admin_path ? l($bundles[$entity_type][$bundle]['label'], $admin_path . '/fields') : $bundles[$entity_type][$bundle]['label'];

        // Add the item count.
        $table = 'field_data_' . $field_name;
        $count = db_query("SELECT count(entity_id) FROM {$table}")->fetchField();
        $rows[$field_name]['data']['count'] = $count;
        $rows[$field_name]['data']['recommendation'] = '';
        if ($count == 0) {
          $rows[$field_name]['class'] = array('warning');
          $rows[$field_name]['data']['recommendation'] = t('Delete this field?');
        }
      }
    }
  }
  foreach ($rows as $field_name => $cell) {
    $rows[$field_name]['data']['bundles'] = implode(', ', $cell['data']['bundles']);
  }
  if (empty($rows)) {
    $output = t('No fields have been defined yet.');
  }

  $field_data = array(
    'title' => t('Field data'),
    'info' => t('Any fields with zero values could be removed.'),
    'header' => array(t('Field name'), t('Field type'), t('Module'), t('Used in'), t('Number of values'), t('Recommendation')),
    'rows' => $rows,
  );
  $renderable['fields'] = backdrop_upgrade_status_report_data($field_data);

  // Updtates defaults based on changed info from above.
  $renderable['intro']['backdrop_upgrade_status_todo']['#default_value'] = $todo_default;

  // Allow other modules to add things to the reports page.
  drupal_alter('backdrop_upgrade_status_report', $renderable);

  return system_settings_form($renderable);
}

/**
 * Creates a fieldset with tabular data for the reports page.
 *
 * @param array $variables
 *   Contains the following keys:
 *   - title
 *   - info
 *   - header
 *   - data
 *
 * @return array
 *   Renderable fieldset.
 */
function backdrop_upgrade_status_report_data($variables) {
  $title = $variables['title']; // has t().
  $info = $variables['info']; // has t().
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
 * Implements hook_backdrop_upgrade_status_report_alter() on behalf of
 * paragraphs module.
 */
function paragraphs_backdrop_upgrade_status_report_alter(&$renderable) {
  // Get all the taxonomy terms in the system.
  $rows = array();
  $bundles = paragraphs_bundle_load();
  foreach ($bundles as $bundle) {
    $count = db_query("SELECT count(item_id) FROM {paragraphs_item} WHERE bundle = :machine", array(':machine' => $bundle->bundle))->fetchField();
    $row = array(
      'data' => array(
        'type' => $bundle->name,
        'machine' => $bundle->bundle,
        'count' => $count,
      ),
    );
    if ($count == 0) {
      $row['class'] = array('warning');
    }
    $rows[] = $row;
  }
  $paragraph_data = array(
    'title' => t('Paragraph content'),
    'info' => t('Any paragraph types with zero items could be deleted.'),
    'header' => array(t('Paragraph type'), t('Machine name'), t('Number of paragraphs')),
    'rows' => $rows,
  );
  $renderable['paragraphs'] = backdrop_upgrade_status_report_data($paragraph_data);
}

/**
 * Implements hook_backdrop_upgrade_status_report_alter() on behalf of image
 * module.
 */
function image_backdrop_upgrade_status_report_alter(&$renderable) {
  // Get all the taxonomy terms in the system.
  $rows = array();
  $collapsed = TRUE;
  $styles = image_styles();
  foreach ($styles as $style) {
    $storage = t('In database');
    $editable = (bool) ($style['storage'] & IMAGE_STORAGE_EDITABLE);
    if (!$editable) {
      $storage = t('In code');
    }
    $row = array(
      'data' => array(
        'label' => $style['label'],
        'machine' => $style['name'],
        'storage' => $storage,
        'recommendation' => '',
      ),
    );
    if (!$editable) {
      $collapsed = FALSE;
      $row['class'] = array('warning');
      $text = t('Overide defaults for this image style');
      $row['data']['recommendation'] = $text;
      if (user_access('administer image styles')) {
        $href = 'admin/config/media/image-styles/edit/' . $style['name'];
        $options = array('query' => array('destination' => current_path()));
        $link = l($text, $href, $options);
        $row['data']['recommendation'] = $link;
      }
    }
    $rows[] = $row;
  }
  $image_style_data = array(
    'title' => t('Image Styles'),
    'info' => t('Image styles need to be stored in the databse for a smooth upgrade.'),
    'header' => array(t('Style Name'), t('Machine name'), t('Storage state'), t('Recommendation')),
    'rows' => $rows,
    'collapsed' => $collapsed,
  );
  $renderable['image_styles'] = backdrop_upgrade_status_report_data($image_style_data);

  // Mark all image_styles as reviewed when all are in the database.
  if ($collapsed && !isset($renderable['intro']['backdrop_upgrade_status_todo']['#default_value']['image_styles'])) {
    $renderable['intro']['backdrop_upgrade_status_todo']['#default_value']['image_styles'] = 'image_styles';
  }
}

/**
 * Implements hook_backdrop_upgrade_status_report_alter() on behalf of views
 * module.
 */
function views_backdrop_upgrade_status_report_alter(&$renderable) {
  // Get all the taxonomy terms in the system.
  $rows = array();
  $collapsed = TRUE;
  $views = views_get_all_views();
  foreach ($views as $view) {
    switch ($view->type) {
      case t('Normal'):
      case t('Overridden'):
        $type = t('In database');
        break;
      default:
        $type = t('In code');
        break;
    }
    $row = array(
      'data' => array(
        'name' => $view->human_name,
        'machine' => $view->name,
        'state' => $type,
        'description' => $view->description,
        'recommendation' => '',
      ),
    );

    if ($type == t('In code')) {
      $collapsed = FALSE;
      $row['class'] = array('error');
      $text = t('Edit and save this view.');
      $row['data']['recommendation'] = $text;
      if (user_access('administer views')) {
        $href = 'admin/structure/views/view/' . $view->name . '/edit';
        $options = array('query' => array('destination' => current_path()));
        $link = l($text, $href, $options);
        $row['data']['recommendation'] = $link;
      }
    }
    $rows[] = $row;
  }
  $view_data = array(
    'title' => t('Views data'),
    'info' => t('All views should show STORAGE STATE as <strong>In database</strong> in order to be properly upgraded into Backdrop CMS. Note: You will first need to make the <em>Save</em> button appear on the Edit View form. Click on any setting to open a modal, then click <em>Cancel</em> to close the modal. The <em>Save</em> button will be at the top right.'),
    'header' => array(t('View name'), t('Machine name'), t('Storage state'), t('Description'), t('Recommendation')),
    'rows' => $rows,
    'collapsed' => $collapsed,
  );
  $renderable['views'] = backdrop_upgrade_status_report_data($view_data);

  // Mark all views as reviewed when all are in the database.
  if ($collapsed && !isset($renderable['intro']['backdrop_upgrade_status_todo']['#default_value']['views'])) {
    $renderable['intro']['backdrop_upgrade_status_todo']['#default_value']['views'] = 'views';
  }
}

/**
 * Implements hook_backdrop_upgrade_status_report_alter() on behalf of
 * features module.
 */
function features_backdrop_upgrade_status_report_alter(&$renderable) {

  // Add a checkbox to the todo list.

  // Add a fieldset to the report.
  $rows = array();
  $collapsed = TRUE;
  module_load_include('inc', 'features', 'features.admin');
  $default_text = array(
    FEATURES_OVERRIDDEN => t('Overridden'),
    FEATURES_DEFAULT => t('Default'),
    FEATURES_NEEDS_REVIEW => t('Needs review'),
    FEATURES_REBUILDING => t('Rebuilding'),
    FEATURES_REBUILDABLE => t('Rebuilding'),
    FEATURES_CONFLICT => t('Conflict'),
    FEATURES_DISABLED => t('Disabled'),
    FEATURES_CHECKING => t('Checking...'),
  );

  $features = _features_get_features_list();
  foreach ($features as $machine => $feature) {
    // Only include enabled features.
    if ($feature->status == 1) {
      $row = array(
        'data' => array(
          'name' => $feature->info['name'],
          'machine' => $machine,
          'state' => $default_text[$feature->status],
          'components' => implode(', ', $feature->components),
          'description' => $feature->info['description'],
          'recommendation' => '',
        ),
      );

      if (module_exists('unfeaturize')) {
        $href = 'admin/config/development/unfeaturize';
        $row['data']['recommendation'] = l(t('Use unfeaturize to restore this configuration to the database.'), $href);
      }
      else {
        $href = 'https://drupal.org/project/unfeaturize';
        $row['data']['recommendation'] = t('Install the <a href="!url">Unfeaturize</a> module.', array('!url' => $href));
      }

      $problem = FALSE;
      $problem_components = array();
      if (module_exists('unfeaturize')) {
        unfeaturize_include();
        foreach ($feature->components as $component) {
          if (!features_hook($component, 'unfeaturize_restore')) {
            $problem_components[] = $component;
          }
        }
      }

      // Remove the components that won't be a problem.
      $not_problem = array('ctools', 'features_api', 'dependencies');
      foreach ($not_problem as $component) {
        $key = array_search($component, $problem_components);
        if ($key !== FALSE) {
          unset($problem_components[$key]);
        }
      }

      // If any problem remains, note it here.
      if (!empty($problem_components)) {
        $problem = TRUE;
      }

      if ($problem) {
        $row['class'] = array('error');
        $list = implode(', ', $problem_components);
        $row['data']['recommendation'] = t('The following components are not yet supported by unfeaturize: !list. You will need to determine how to move these items into the database.', array('!list' => $list));
      }

      $rows[] = $row;
    }
  }

  $collapsed = FALSE;
  $replacements = array('!unfeatureized' => l(t('unfeatureized'), 'https://www.drupal.org/project/unfeaturize'));
  $feature_data = array(
    'title' => t('Features data'),
    'info' => t('All configuration from Features will need to be in the database in order to be properly upgraded to Backdrop CMS. Features may need to be !unfeatureized.', $replacements),
    'header' => array(t('Name'), t('Machine name'), t('Storage state'), t('Components'), t('Description'), t('Recommendation')),
    'rows' => $rows,
    'collapsed' => $collapsed,
  );
  $renderable['features'] = backdrop_upgrade_status_report_data($feature_data);
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
  $drupal_url = 'https://drupal.org/pr';
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

/**
 * Implements hook_form_FORM_ID_alter().
 *
 *   @todo  none of this works ?!?
 */
function backdrop_upgrade_status_form_views_ui_edit_form_alter(&$form, &$form_state, $view, $display_id) {
  // Don't hide the Save button, so that people can save views into the DB.
  $key = array_search('js-hide', $form['actions']['#attributes']['class']);
  if ($key !== FALSE) {
    unset($form['actions']['#attributes']['class'][$key]);
  }

  // Add CSS to force the Save button to display.
  //$path = drupal_get_path('module', 'backdrop_upgrade_status');
  //$form['#attached']['css'][$path . '/css/views-fix.css'] = array();

  // Add JS to remove the js-hide class.
  //$form['#attached']['js'][] = $path . '/js/views-fix.js';
  //dpm($form['#attached']['js']);

  // Mark the view as changed, and invalidate the cache.
  $form_state['view']->changed = TRUE;
  views_invalidate_cache();
}
