<?php
/**
 * Hooks implemented on behalf of field module.
 */

/**
 * Implements node_backdrop_upgrade_status_todo().
 */
function field_backdrop_upgrade_status_todo() {
  return array(
    'field' => array(
      'review' => 'fields',
      'description' => 'Remove any fields that are not in use, or not wanted in Backdrop.',
      'optional' => TRUE,
      'weight' => 0,
    ),
  );
}

/**
 * Returns all content needed for the Backdrop Upgrade Status report.
 *
 * @param array $$renderable
 *   The rest of the form.
 * @param boolean $todo_status
 *   Value of the todo checkbox based on the current state of the site.
 *
 * @return array
 *   An array containing all the data needed to build a table on the report,
 *   containing the following keys:
 *   - title:
 *   - info:
 *   - header:
 *   - rows:
 *   - collapsed:
 *   - todo_status:
 */
function field_backdrop_upgrade_status_report($todo_status = FALSE) {
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
        $count = db_query("SELECT count(entity_id) FROM {" . $table . "}")->fetchField();
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

  // Get current state.
  $todo_status = backdrop_upgrade_status_get_state('field');
  $collapsed = $todo_status;

  $return = array(
    'machine' => 'field',
    'title' => t('Field data'),
    'info' => t('Any fields with zero values could be removed.'),
    'header' => array(t('Field name'), t('Field type'), t('Module'), t('Used in'), t('Number of values'), t('Recommendation')),
    'rows' => $rows,
    'empty' => t('No custom fields have been detected.'),
    'collapsed' => $collapsed,
    'todo_status' => $todo_status,
  );

  return $return;
}
