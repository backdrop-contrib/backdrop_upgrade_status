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
 * Implements hook_backdrop_upgrade_status_report_alter().
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
