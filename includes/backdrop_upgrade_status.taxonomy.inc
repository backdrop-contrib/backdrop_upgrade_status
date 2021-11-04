<?php
/**
 * Hooks implemented on behalf of taxonomy module.
 */

/**
 * Implements hook_backdrop_upgrade_status_todo().
 */
function taxonomy_backdrop_upgrade_status_todo() {
  return array(
    'taxonomy' => array(
      'review' => 'taxonomy',
      'description' => 'Remove terms or vocabularies that are not in use, or wanted in Backdrop.',
      'optional' => TRUE,
      'weight' => 0,
    ),
  );
}

/**
 * Implements hook_backdrop_upgrade_status_report_alter().
 */
function taxonomy_backdrop_upgrade_status_report_alter(&$renderable) {
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
}
