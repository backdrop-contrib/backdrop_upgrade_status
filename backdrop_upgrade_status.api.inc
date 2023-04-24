<?php
/**
 * @file
 * API: Hooks provided by this module.
 */

/**
 * Returns all content needed for the Backdrop Upgrade Status report.
 *
 * @param array $renderable
 *
 * @return array
 *   An array containing all the data needed to build a table on the report,
 *   containing the following keys:
 *   - machine:
 *   - title:
 *   - info:
 *   - header:
 *   - rows:
 *   - empty:
 *   - collapsed:
 *   - todo_status:
 */
function hook_backdrop_upgrade_status_report() {
  $return = array(
    'machine' => 'profile',
    'title' => t('Profile / Distribution'),
    'info' => t('This is the profile that was used when the site was first installed.'),
    'header' => array(t('Profile name'), t('Enabled'), t('Distribution'), t('Backdrop status'), t('Recommendation')),
    'rows' => $profile_rows,
    'empty' => '',
    'collapsed' => $collapsed,
  );

  return $return;
}

/**
 * Implements hook_backdrop_upgrade_status_report_alter().
 *
 * @param array &$renderable
 *   The renderable array used as the form on the overview page.
 */
function hook_backdrop_upgrade_status_report_alter(&$renderable) {
  // @todo
}

/**
 * Implements hook_backdrop_upgrade_status_scan().
 *
 * @return array
 *   For each key in hook_backdrop_upgrade_status_todo, return TRUE or FALSE.
 *
 * @see hook_backdrop_upgrade_status_todo().
 */
function hook_backdrop_upgrade_status_scan() {
  $styles = image_styles();
  foreach ($styles as $style) {
    $editable = (bool) ($style['storage'] && IMAGE_STORAGE_EDITABLE);
    if (!$editable) {
      return array('image' => FALSE);
    }
  }

  return array('image' => TRUE);
}

/**
 * Implements hook_backdrop_upgrade_status_todo().
 *
 * @return array
 *   Todo item info, keyed by machine name. Info contains the following keys.
 *     - review: Human-readable name of the item to review.
 *     - description: Description of the item to review.
 *     - optional: Boolean, TRUE if the item shoudl be labeled as optional.
 */
function hook_backdrop_upgrade_status_todo() {
  return array(
    'profile' => array(
      'review' => 'system profile',
      'description' => 'Check that a Backdrop version is available.',
      'optional' => TRUE,
    ),
  );
}

/**
 * Implements hook_backdrop_upgrade_status_todo_alter().
 *
 * @param array $todo_items
 *   Todo item info, keyed by machine name. Info contains the following keys.
 *   - review: Human-readable name of the item to review.
 *   - description: Description of the item to review.
 *   - optional: Boolean, TRUE if the item shoudl be labeled as optional.
 */
function backdrop_upgrade_status_todo_alter(&$todo_items) {
  unset($todo_items['profile']);
}
