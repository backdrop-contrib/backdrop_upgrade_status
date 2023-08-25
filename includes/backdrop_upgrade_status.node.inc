<?php
/**
 * Hooks implemented on behalf of node module.
 */

/**
 * Implements node_backdrop_upgrade_status_todo().
 */
function node_backdrop_upgrade_status_todo() {
  return array(
    'node' => array(
      'review' => 'content types',
      'description' => 'Remove any content types that are not in use, or not wanted in Backdrop.',
      'optional' => TRUE,
      'weight' => 0,
    ),
  );
}

/**
 * Implements hook_backdrop_upgrade_status_report().
 */
function node_backdrop_upgrade_status_report($todo_status = FALSE) {
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

  // Get current state.
  $todo_status = backdrop_upgrade_status_get_state('node');
  $collapsed = $todo_status;

  $return = array(
    'machine' => 'node',
    'title' => t('Content types'),
    'info' => t('Any content types with zero items could be deleted.'),
    'header' => array(t('Content type'), t('Machine name'), t('Number of items'), t('Recommendation')),
    'rows' => $rows,
    'empty' => t('No custom node types have been detected.'),
    'collapsed' => $collapsed,
    'todo_status' => $todo_status,
  );

  return $return;
}
