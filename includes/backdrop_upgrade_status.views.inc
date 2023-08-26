<?php
/**
 * Hooks implemented on behalf of views module.
 */

/**
 * Implements hook_backdrop_upgrade_status_todo().
 */
function views_backdrop_upgrade_status_todo() {
  return array(
    'views' => array(
      'review' => 'views',
      'description' => 'Configuration must be in the database to convert automatically during upgrade.',
    ),
  );
}

/**
 * Implements hook_backdrop_upgrade_status_report().
 */
function views_backdrop_upgrade_status_report($todo_status = FALSE) {
  // Get all the taxonomy terms in the system.
  $rows = array();

  // Get current state.
  $todo_status = backdrop_upgrade_status_get_state('views');
  $collapsed = $todo_status;

  $views = views_get_all_views();
  foreach ($views as $view) {

    $row = array(
      'data' => array(
        'name' => $view->human_name,
        'machine' => $view->name,
        'state' => '',
        'description' => $view->description,
        'recommendation' => '',
      ),
    );

    // Mark disabled views with a warning
    if ( $view->disabled ) {
      $row['class'] = array('warning');
      $row['data']['state'] = t('Disabled');
      $text = t('If required, enable this view.');
      $row['data']['recommendation'] = $text;
      if (user_access('administer views')) {
        $href = 'admin/structure/views/view/' . $view->name . '/edit';
        $options = array('query' => array('destination' => current_path()));
        $link = l($text, $href, $options);
        $row['data']['recommendation'] = $link;
      }
    }

    else {

      $in_code = FALSE;
      // Show views that need to be added to DB.
      switch ($view->type) {
        case t('Normal'):
        case t('Overridden'):
          $type = t('In database');
        break;
        default:
          $type = t('In code');
          $in_code = TRUE;
        break;
      }

      $row['data']['state'] = $type;

      if ($in_code) {
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
    }

    $rows[] = $row;
  }

  // Mark all views as reviewed when all are in the database.
  if ($collapsed && !isset($renderable['intro']['backdrop_upgrade_status_todo']['#default_value']['views'])) {
    $todo_status = TRUE;
  }

  $return = array(
    'machine' => 'views',
    'title' => t('Views data'),
    'info' => t('All views should show STORAGE STATE as <strong>In database</strong> in order to be properly upgraded into Backdrop CMS. Note: In order to return a view to the database you will first need to make the <em>Save</em> button appear on the Edit View form. On the form, click on any setting to open a modal, then click <em>Cancel</em> to close the modal. The <em>Save</em> should appear at the top right.'),
    'header' => array(t('View name'), t('Machine name'), t('Storage state'), t('Description'), t('Recommendation')),
    'rows' => $rows,
    'empty' => t('No enabled custom views have been detected.'),
    'collapsed' => $collapsed,
    'todo_status' => $todo_status,
  );

  return $return;
}
