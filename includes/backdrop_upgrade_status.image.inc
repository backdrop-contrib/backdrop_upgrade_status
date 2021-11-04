<?php
/**
 * Hooks implemented on behalf of image module.
 */

/**
 * Implements hook_backdrop_upgrade_status_todo().
 */
function image_backdrop_upgrade_status_todo() {
  return array(
    'image' => array(
      'review' => 'image styles',
      'description' => 'Configuration must be in the database to convert automatically durring upgrade.',
      'optional' => TRUE,
      'weight' => 0,
    ),
  );
}

/**
 * Implements hook_backdrop_upgrade_status_report().
 */
function image_backdrop_upgrade_status_report($todo_status = FALSE) {
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

  // Mark all image_styles as reviewed when all are in the database.
  $todo_status = FALSE;
  if ($collapsed && !isset($renderable['intro']['backdrop_upgrade_status_todo']['#default_value']['image_styles'])) {
    $todo_status = TRUE;
  }

  $return = array(
    'title' => t('Image Styles'),
    'info' => t('Image styles need to be stored in the databse for a smooth upgrade.'),
    'header' => array(t('Style Name'), t('Machine name'), t('Storage state'), t('Recommendation')),
    'rows' => $rows,
    'collapsed' => $collapsed,
    'todo_status' => $todo_status,
    'machine' => 'image',
  );

  return $return;
}
