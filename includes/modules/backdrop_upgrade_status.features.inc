<?php
/**
 * Hooks implemented on behalf of features module.
 */

/**
 * Implements hook_backdrop_upgrade_status_todo().
 */
function features_backdrop_upgrade_status_todo() {
  return array(
    'features' => array(
      'review' => 'features',
      'description' => 'Configuration must be in the database to convert automatically during upgrade.',
      'weight' => 9,
    ),
  );
}

/**
 * Implements hook_backdrop_upgrade_status_report().
 */
function features_backdrop_upgrade_status_report($todo_status = FALSE) {
  // Add a checkbox to the todo list.

  // Add a fieldset to the report.
  $rows = array();

  // Get current state.
  $todo_status = backdrop_upgrade_status_get_state('features');
  $collapsed = $todo_status;

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

  $replacements = array('!unfeaturized' => l(t('unfeaturized'), 'https://www.drupal.org/project/unfeaturize'));
  $info = t('All configuration from Features will need to be in the database in order to be properly upgraded to Backdrop CMS. Features may need to be !unfeaturized.', $replacements);

  $return = array(
    'machine' => 'features',
    'title' => t('Features data'),
    'info' => $info,
    'header' => array(t('Name'), t('Machine name'), t('Storage state'), t('Components'), t('Description'), t('Recommendation')),
    'rows' => $rows,
    'empty' => t('No feature modules have been detected.'),
    'collapsed' => $collapsed,
    'todo_status' => $todo_status,
  );

  return $return;
}

