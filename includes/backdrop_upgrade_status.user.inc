<?php
/**
 * Hooks implemented on behalf of node module.
 */

/**
 * Implements node_backdrop_upgrade_status_todo().
 */
function user_backdrop_upgrade_status_todo() {
  return array(
    'user' => array(
      'review' => 'user accounts',
      'description' => 'Remove old or blocked accounts that are not in use, or not wanted in Backdrop.',
      'optional' => TRUE,
      'weight' => 0,
    ),
  );
}

/**
 * Implements hook_backdrop_upgrade_status_report_alter().
 */
function user_backdrop_upgrade_status_report($todo_status = FALSE) {
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

  // @todo add $collapsed
  $collapsed = FALSE;

  // @todo add $todo_status
  $todo_status = FALSE;

  $return = array(
    'machine' => 'user',
    'title' => t('User account data'),
    'info' => t('This is a summary of the user accounts on the site.'),
    'header' => array(t('User accounts'), t('Status'), t('Number of users')),
    'rows' => $rows,
    'empty' => '',
    'collapsed' => $collapsed,
    'todo_status' => $todo_status,
  );

  return $return;
}
