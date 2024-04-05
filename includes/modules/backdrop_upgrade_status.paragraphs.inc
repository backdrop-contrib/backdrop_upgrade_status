<?php
/**
 * Hooks implemented on behalf of image module.
 */

/**
 * Implements hook_backdrop_upgrade_status_todo().
 */
function paragraphs_backdrop_upgrade_status_todo() {
  return array(
    'paragraphs' => array(
      'review' => 'paragraphs',
      'description' => 'Remove paragraphs that are not wanted anymore.',
      'optional' => TRUE,
    ),
  );
}

/**
 * Implements hook_backdrop_upgrade_status_report().
 */
function paragraphs_backdrop_upgrade_status_report($todo_status = FALSE) {
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

  // Get current state.
  $todo_status = backdrop_upgrade_status_get_state('paragraphs');
  $collapsed = $todo_status;

  $return = array(
    'machine' => 'paragraphs',
    'title' => t('Paragraph content'),
    'info' => t('Any paragraph types with zero items could be deleted.'),
    'header' => array(t('Paragraph type'), t('Machine name'), t('Number of paragraphs')),
    'rows' => $rows,
    'empty' => t('No paragraph types have been detected.'),
    'collapsed' => $collapsed,
    'todo_status' => $todo_status,
  );

  return $return;
}
