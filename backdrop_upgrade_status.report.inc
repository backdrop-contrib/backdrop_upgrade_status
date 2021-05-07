<?php

/**
 * @file
 * Code required only when rendering the available updates report.
 */

module_load_include('inc', 'update', 'update.report');

/**
 * Page callback: Generates a page about the update status of projects.
 *
 * @see backdrop_upgrade_status_menu()
 */
function backdrop_upgrade_status_status() {
  // US: Don't pass TRUE to check cache only.
  if ($available = backdrop_upgrade_status_get_available()) {
    module_load_include('inc', 'backdrop_upgrade_status', 'backdrop_upgrade_status.compare');
    $data = backdrop_upgrade_status_calculate_project_data($available);
    return theme('backdrop_upgrade_status_report', array('data' => $data));
  }
  else {
    // US: Inlined _update_no_data().
    // @todo: File an upstream patch for that.
    $destination = drupal_get_destination();
    $data = t('No upgrade information available. <a href="@run_cron">Run cron</a> or <a href="@check_manually">check manually</a>.', array(
      '@run_cron' => url('admin/reports/status/run-cron', array('query' => $destination)),
      '@check_manually' => url('admin/reports/updates/backdrop-upgrade/check', array('query' => $destination)),
    ));
    return theme('backdrop_upgrade_status_report', array('data' => $data));
  }
}

/**
 * Returns HTML for the project status report.
 *
 * @param array $variables
 *   An associative array containing:
 *   - data: An array of data about each project's status.
 *
 * @ingroup themeable
 */
function theme_backdrop_upgrade_status_report($variables) {
  $data = $variables['data'];

  $last = variable_get('backdrop_upgrade_status_last_check', 0);
  $output = theme('backdrop_upgrade_status_last_check', array('last' => $last));

  // US: Load Update module's data as a fallback in case there is no release info
  // for a project (so we can link to the project).
  $current_data = update_get_available(TRUE);

  if (!is_array($data)) {
    $output .= '<p>' . $data . '</p>';
    return $output;
  }

  $header = array();
  $rows = array();

  // US: We don't do anything with notifications in this module.
#  $notification_level = variable_get('update_notification_threshold', 'all');

  // Create an array of status values keyed by module or theme name, since
  // we'll need this while generating the report if we have to cross reference
  // anything (e.g. subthemes which have base themes missing an update).
  foreach ($data as $project) {
    if (empty($project['includes'])){
      $project['includes'] = array();
    }
    foreach ($project['includes'] as $key => $name) {
      $status[$key] = $project['status'];
    }
  }

  foreach ($data as $project) {
    // Skip future version replacements and this module, which would cause
    // confusion if it showed up.
    if (isset($project['api_version']) || $project['name'] == 'backdrop_upgrade_status' || $project['name'] == 'drupal') {
      continue;
    }

    switch ($project['status']) {
      // US: Stable releases and projects moved into core are also good news.
      case UPDATE_CURRENT:
      case BACKDROP_UPGRADE_STATUS_STABLE:
      case BACKDROP_UPGRADE_STATUS_CORE:
      // @todo: Colour obsolete modules according to their replacement project
      // info in $data.
      case BACKDROP_UPGRADE_STATUS_OBSOLETE:
        $class = 'ok';
        $icon = theme('image', array('path' => 'misc/watchdog-ok.png', 'width' => 18, 'height' => 18, 'alt' => t('ok'), 'title' => t('ok')));
        break;
      case UPDATE_UNKNOWN:
      case UPDATE_FETCH_PENDING:
      case UPDATE_NOT_FETCHED:
        if (isset($project['replaced_by'])) {
          $class = 'warning';
          $icon = theme('image', array('path' => 'misc/watchdog-warning.png', 'width' => 18, 'height' => 18, 'alt' => t('warning'), 'title' => t('warning')));
        }
        else {
          $class = 'unknown';
          // US: Unknown means not ported; thus, use an error icon.
          $icon = theme('image', array('path' => 'misc/watchdog-error.png', 'width' => 18, 'height' => 18, 'alt' => t('error'), 'title' => t('error')));
        }
        break;
      // US: Not used/not possible here.
      case UPDATE_NOT_SECURE:
      case UPDATE_REVOKED:
      case UPDATE_NOT_SUPPORTED:
        $class = 'error';
        $icon = theme('image', array('path' => 'misc/watchdog-error.png', 'width' => 18, 'height' => 18, 'alt' => t('error'), 'title' => t('error')));
        break;
      // US: It doesn't make sense to output a whole page of warning symbols,
      // so we just colorize as a warning without the icon. This allows us to
      // warn about projects moved into core and other important info.
      case BACKDROP_UPGRADE_STATUS_DEVELOPMENT:
        $class = 'warning';
        $icon = '';
        break;
      case BACKDROP_UPGRADE_STATUS_OBSOLETE:
        $row .= '<span class="current">'. t('Made obsolete') .'</span>';
        break;

      case UPDATE_NOT_CHECKED:
      case UPDATE_NOT_CURRENT:
      default:
        $class = 'warning';
        $icon = theme('image', array('path' => 'misc/watchdog-warning.png', 'width' => 18, 'height' => 18, 'alt' => t('warning'), 'title' => t('warning')));
        break;
    }
    // US: Special handling for project moved into core.
    if ($project['status'] == BACKDROP_UPGRADE_STATUS_CORE && empty($project['in_core_complete'])) {
      $icon = theme('image', array('path' => 'misc/watchdog-warning.png', 'width' => 18, 'height' => 18, 'alt' => t('warning'), 'title' => t('warning')));
    }

    $row = '<div class="version-status">';
    $status_label = theme('backdrop_upgrade_status_status_label', array('status' => $project['status'], 'project' => $project));
    $row .= !empty($status_label) ? $status_label : check_plain($project['reason']);
    $row .= '<span class="icon">' . $icon . '</span>';
    $row .= "</div>\n";

    $row .= '<div class="project">';
    if (isset($project['title'])) {
      if (isset($project['link'])) {
        $row .= l($project['title'], $project['link']);
      }
      else {
        $row .= check_plain($project['title']);
      }
    }
    // US: Couldn't find this project's data for the next version of Drupal core.
    // Let's try the current one instead.
    elseif (isset($current_data[$project['name']]) && isset($current_data[$project['name']]['title'])) {
      if (isset($current_data[$project['name']]['link'])) {
        $row .= l($current_data[$project['name']]['title'], $current_data[$project['name']]['link']);
      }
      else {
        $row .= check_plain($current_data[$project['name']]['title']);
      }
    }
    // Otherwise, just print the name.
    else {
      $row .= check_plain($project['name']);
    }
    $row .= ' ' . check_plain($project['existing_version']);
    if ($project['install_type'] == 'dev' && !empty($project['datestamp'])) {
      $row .= ' <span class="version-date">(' . format_date($project['datestamp'], 'custom', 'Y-M-d') . ')</span>';
    }
    $row .= "</div>\n";

    // US: Special handling for projects moved into core.
    if (!empty($project['in_core_note'])) {
      $row .= "<div class=\"core-notice\">\n";
      $row .=   '<p>';
      $row .=     '<strong>'. t('In Backdrop core since @version', array('@version' => $project['in_core_since'])) . '</strong><br>';
      $row .=      $project['in_core_note'];
      if (isset($project['in_core_warning'])) {
        $row .=   ' <em>' . $project['in_core_warning'] . '</em>';
      }
      $row .=   '</p>';
      $row .= '</div>';
    }

    $versions_inner = '';
    $security_class = array();
    $version_class = array();
    if (isset($project['recommended']) && (empty($project['in_core_complete']))) {
      if ($project['status'] != UPDATE_CURRENT || $project['existing_version'] !== $project['recommended']) {

        // First, figure out what to recommend.
        // If there's only 1 security update and it has the same version we're
        // recommending, give it the same CSS class as if it was recommended,
        // but don't print out a separate "Recommended" line for this project.
        if (!empty($project['security updates']) && count($project['security updates']) == 1 && $project['security updates'][0]['version'] === $project['recommended']) {
          $security_class[] = 'version-recommended';
          $security_class[] = 'version-recommended-strong';
        }
        else {
          $version_class[] = 'version-recommended';
          // Apply an extra class if we're displaying both a recommended
          // version and anything else for an extra visual hint.
          if ($project['recommended'] !== $project['latest_version']
              || !empty($project['also'])
              || ($project['install_type'] == 'dev'
                  && isset($project['dev_version'])
                  && $project['latest_version'] !== $project['dev_version']
                  && $project['recommended'] !== $project['dev_version'])
              || (isset($project['security updates'][0])
                  && $project['recommended'] !== $project['security updates'][0])
          ) {
            $version_class[] = 'version-recommended-strong';
          }
          $versions_inner .= theme('update_version', array('version' => $project['releases'][$project['recommended']], 'tag' => t('Recommended version:'), 'class' => $version_class));
        }

        // Now, print any security updates.
        if (!empty($project['security updates'])) {
          $security_class[] = 'version-security';
          foreach ($project['security updates'] as $security_update) {
            $versions_inner .= theme('update_version', array('version' => $security_update, 'tag' => t('Security update:'), 'class' => $security_class));
          }
        }
      }

      if ($project['recommended'] !== $project['latest_version']) {
        $versions_inner .= theme('update_version', array('version' => $project['releases'][$project['latest_version']], 'tag' => t('Latest version:'), 'class' => array('version-latest')));
      }
      if ($project['install_type'] == 'dev'
          && $project['status'] != UPDATE_CURRENT
          && isset($project['dev_version'])
          && $project['recommended'] !== $project['dev_version']) {
        $versions_inner .= theme('update_version', array('version' => $project['releases'][$project['dev_version']], 'tag' => t('Development version:'), 'class' => array('version-latest')));
      }
    }

    if (isset($project['also'])) {
      foreach ($project['also'] as $also) {
        $versions_inner .= theme('update_version', array('version' => $project['releases'][$also], 'tag' => t('Also available:'), 'class' => array('version-also-available')));
      }
    }

    if (!empty($versions_inner)) {
      $row .= "<div class=\"versions\">\n" . $versions_inner . "</div>\n";
    }
    $row .= "<div class=\"info\">\n";
    if (!empty($project['extra'])) {
      $row .= '<div class="extra">' . "\n";
      foreach ($project['extra'] as $key => $value) {
        $row .= '<div class="' . implode(' ', $value['class']) . '">';
        $row .= check_plain($value['label']) . ': ';
        $row .= drupal_placeholder($value['data']);
        $row .= "</div>\n";
      }
      $row .= "</div>\n";  // extra div.
    }

    if (isset($project['replaced_by'])) {
      $row .= '<div class="includes">';
      $replacements = array();
      foreach ($project['replaced_by'] as $replacement) {
        $replacements[] = t('!name @version', array('!name' => l($data[$replacement['name']]['title'], $data[$replacement['name']]['link']), '@version' => $data[$replacement['name']]['recommended']));
      }
      $replaced = implode(', ', $replacements);
      $row .= t('Replaced by: !replaced', array('!replaced' => $replaced));
      $row .= "</div>\n";
    }

    if (!empty($project['disabled'])) {
      sort($project['disabled']);
      // Make sure we start with a clean slate for each project in the report.
      $includes_items = array();
      $row .= t('Includes:');
      $includes_items[] = t('Enabled: %includes', array('%includes' => implode(', ', $project['includes'])));
      $includes_items[] = t('Disabled: %disabled', array('%disabled' => implode(', ', $project['disabled'])));
      $row .= theme('item_list', array('items' => $includes_items));
    }
    else {
      $row .= '<div class="includes">';
      sort($project['includes']);
      $row .= t('Includes: %includes', array('%includes' => implode(', ', $project['includes'])));
    }
    $row .= "</div>\n";

    if (!empty($project['base_themes'])) {
      $row .= '<div class="basethemes">';
      asort($project['base_themes']);
      $base_themes = array();
      foreach ($project['base_themes'] as $base_key => $base_theme) {
        switch ($status[$base_key]) {
          case UPDATE_NOT_SECURE:
          case UPDATE_REVOKED:
          case UPDATE_NOT_SUPPORTED:
            $base_themes[] = t('%base_theme (!base_label)', array('%base_theme' => $base_theme, '!base_label' => theme('backdrop_upgrade_status_status_label', array('status' => $status[$base_key]))));
            break;

          default:
            $base_themes[] = drupal_placeholder($base_theme);
        }
      }
      $row .= t('Depends on: !basethemes', array('!basethemes' => implode(', ', $base_themes)));
      $row .= "</div>\n";
    }

    if (!empty($project['sub_themes'])) {
      $row .= '<div class="subthemes">';
      sort($project['sub_themes']);
      $row .= t('Required by: %subthemes', array('%subthemes' => implode(', ', $project['sub_themes'])));
      $row .= "</div>\n";
    }

    $row .= "</div>\n"; // info div.

    if (!isset($rows[$project['project_type']])) {
      $rows[$project['project_type']] = array();
    }
    $row_key = isset($project['title']) ? drupal_strtolower($project['title']) : drupal_strtolower($project['name']);
    $rows[$project['project_type']][$row_key] = array(
      'class' => array($class),
      'data' => array($row),
    );
  }

  $project_types = array(
    // 'core' => t('Drupal core'),
    'module' => t('Modules'),
    'theme' => t('Themes'),
    'module-disabled' => t('Disabled modules'),
    'theme-disabled' => t('Disabled themes'),
  );
  foreach ($project_types as $type_name => $type_label) {
    if (!empty($rows[$type_name])) {
      ksort($rows[$type_name]);
      $output .= "\n<h3>" . $type_label . "</h3>\n";
      $output .= theme('table', array('header' => $header, 'rows' => $rows[$type_name], 'attributes' => array('class' => array('update'))));
    }
  }
  drupal_add_css(drupal_get_path('module', 'update') . '/update.css');
  return $output;
}

/**
 * Returns HTML for a label to display for a project's update status.
 *
 * @param array $variables
 *   An associative array containing:
 *   - status: The integer code for a project's current update status.
 *   // US: Add project data as well so we can determine type of release.
 *   // @todo File upstream patch?
 *   - project: Project data.
 *
 * @see update_calculate_project_data()
 * @ingroup themeable
 */
function theme_backdrop_upgrade_status_status_label($variables) {
  $project = $variables['project'];

  switch ($variables['status']) {
    // US: Not applicable.
#    case UPDATE_NOT_SECURE:
#      return '<span class="security-error">' . t('Security update required!') . '</span>';

    // US: Not applicable.
#    case UPDATE_REVOKED:
#      return '<span class="revoked">' . t('Revoked!') . '</span>';

    // Although unsupported releases should actually be unsupported, we treat
    // them like development releases, since many maintainers merely use this
    // additional flag to hide the release from novice Drupal users.
    case BACKDROP_UPGRADE_STATUS_DEVELOPMENT:
    case UPDATE_NOT_SUPPORTED:
#      return '<span class="not-supported">' . t('Not supported!') . '</span>';
      // US: Additionally output the "development stage" of a project; alpha,
      // beta, and RC are all treated as in development.
      $type = $project['releases'][$project['recommended']]['version'];
      return '<span class="not-current">'. t('In development: %type', array('%type' => $type)) .'</span>';

      case UPDATE_UNKNOWN:
      case UPDATE_FETCH_PENDING:
      case UPDATE_NOT_FETCHED:
      if (isset($project['replaced_by'])) {
        return '<span class="current">' . t('Replaced in Backdrop') . '</span>';
      }
      return NULL;

    // US: Good news for us means that a stable release is available...
#    case UPDATE_CURRENT:
#      return '<span class="current">' . t('Up to date') . '</span>';
    case BACKDROP_UPGRADE_STATUS_STABLE:
      return '<span class="current">'. t('Available') .'</span>';

    // US: ...or that a module's been moved into core.
    case BACKDROP_UPGRADE_STATUS_CORE:
      return '<span class="current">'. t('In core') .'</span>';
  }
}

/**
 * Generates an informational report showing everything you'll need to upgrade
 * your site from Drupal to Backdrop CMS.
 *
 * @return array
 *   Renderable tables with all the info!
 */
function backdrop_upgrade_status_report() {
  $renderable = array();

  $welcome = array(
    t('This information should prove useful as you are evaluating an upgrade to Backdrop CMS for this site. This report should contain an overview for most of the content on this site.'),
  );

  $renderable['welcome'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . implode('</p><p>', $welcome) . '</p>',
  );

  $renderable['intro'] = array(
    '#type' => 'fieldset',
    '#title' => t('Backdrop Upgrade to-do list'),
    '#description' => t('When you are ready to perform the upgrade, this list should walk you through the steps necessary to prepare your Drupal 7 database.'),
    '#collapsible' => TRUE,
  );
  $default = variable_get('backdrop_upgrade_status_todo', array(1 => TRUE));
  $renderable['intro']['backdrop_upgrade_status_todo'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Prepare for upgrading to Backdrop CMS:'),
    '#options' => array(
      1 => t('Review <a href="#edit-core-modules">core modules</a>: disable and uninstall anthing not in use.'),
      2 => t('Review <a href="#edit-contrib-modules">contrib modules</a>: disable and uninstall anthing not in use.'),
      3 => t('Review <a href="#edit-custom-modules">custom modules</a>: disable and uninstall anthing not in use.'),
      4 => t('Review <a href="#edit-views">views</a>: Configuration must be in the database to convert automatically durung upgrade.'),
      5 => t('Review <a href="#edit-features">features</a>: Configuration must be in the database to convert automatically durung upgrade.'),
      6 => t('(optional) Swicth to the Bartik theme for the upgrade (Useful if you have not yet ported your Drupal 7 theme to Backdrop)'),
      7 => t('(optional) Review <a href="#edit-nodes">content types</a>: remove any content types that are not wanted anymore.'),
      8 => t('(optional) Review <a href="#edit-fields">fields</a>: remove fields that are not wanted anymore.'),
      9 => t('(optional) Review <a href="#edit-terms">taxonomy</a>: remove terms or vocabularies that are not wanted anymore.'),
      10 => t('(optional) Review <a href="#edit-paragraphs">paragraphs</a>: remove paragraphs that are not wanted anymore.'),
      11 => t('(optional) Review <a href="#edit-users">user accounts</a>: remove old or blocked accounts not in use.'),
    ),
    '#default_value' => $default,
  );
  $renderable['intro']['save'] = array(
    '#type' => 'button',
    '#value' => t('Confirm status'),
    '#disabled' => TRUE,
  );


  // Get all module data.
  $projects = system_rebuild_module_data();
  // List of core modules removed from Backdrop, and what to do about it.
  $removed_modules = array(
    'aggregator' => array(
      'class' => 'warning',
      'rec' => t('Add the contrib version of Aggregtor to Backdrop.'),
    ),
    'blog' => array(
      'class' => 'error',
      'rec' => t('Delete content, Convert to custom content type, or port to Backdrop.'),
    ),
    'dashboard' => array(
      'class' => '',
      'rec' => t('Alternaive core module provided. No action required.'),
    ),
    'forum' => array(
      'class' => 'warning',
      'rec' => t('Add the contrib version of Forum to Backdrop.'),
    ),
    'help' => array(
      'class' => 'warning', // Warning, because help is rarely necessary.
      'rec' => t('Disable and Uninstall :module, or port to Backdrop.)'),
    ),
    'openid' => array(
      'class' => 'error',
      'rec' => t('Port :module to Backdrop, or Disable and Uninstall.'),
    ),
    'overlay' => array(
      'class' => 'warning', // Warning, because overlay is rarely necessary.
      'rec' => t('Disable and Uninstall :module, or port to Backdrop.'),
    ),
    'php' => array(
      'class' => 'error',
      'rec' => t('Review all PHP code: delete or change format. Disable and Uninstall PHP Filter module.'),
    ),
    'poll' => array(
      'class' => 'error',
      'rec' => t('Port Poll modile to Backdrop, or Disable and Uninstall.'),
    ),
    'profile' => array(
      'class' => 'warning',
      'rec' => t('Add the contrib version of :module to Backdrop.'),
    ),
    'rdf' => array(
      'class' => 'warning', // Warning, because rdf is rarely necessary.
      'rec' => t('Disable and Uninstall :module, or port to Backdrop.'),
    ),
    'shortcut' => array(
      'class' => 'warning', // Warning, because shortcut is rarely necessary.
      'rec' => t('Disable and Uninstall :module, or port to Backdrop.'),
    ),
    'statistics' => array(
      'class' => 'warning', // Warning, because statistics is rarely necessary.
      'rec' => t('Disable and Uninstall :module, or port to Backdrop.'),
    ),
    'toolbar' => array(
      'class' => '',
      'rec' => t('Alternaive core module provided. No action required.'),
    ),
    'tracker' => array(
      'class' => 'warning', // Warning, because tracker is rarely necessary.
      'rec' => t('Disable and Uninstall :module, or port to Backdrop.'),
    ),
    'trigger' => array(
      'class' => 'warning',
      'rec' => t('Add the contrib version of :module to Backdrop.'),
    )
  );

  // Get backdrop project data.
  if ($available = backdrop_upgrade_status_get_available()) {
    module_load_include('inc', 'backdrop_upgrade_status', 'backdrop_upgrade_status.compare');
    $backdrop_data = backdrop_upgrade_status_calculate_project_data($available);
  }

  $collapsed = TRUE;
  $core_modules = array();
  $core_module_rows = array();
  $contrib_modules = array();
  $contrib_module_rows = array();
  $last_project = '';
  foreach ($projects as $project) {
    $to_skip = array('views_ui', 'backdrop_upgrade_status');
    if (in_array($project->name, $to_skip)) {
      continue;
    }

    if ($project->type == 'module' && $project->status == 1) {

      // Core modules.
      if ($project->info['package'] == 'Core') {
        $core_modules[$project->name] = $project;
        if (array_key_exists($project->name, $removed_modules)) {
          $backdrop_status = t('Removed from core');
          $recommendation = $removed_modules[$project->name]['rec'];
          if (!empty($removed_modules[$project->name]['class'])) {
            $class = $removed_modules[$project->name]['class'];
            $collapsed = FALSE;
          }
        }
        else {
          $backdrop_status = t('In core');
          $recommendation = '';
          $class = '';
        }
        $row = array(
          'data' => array(
            'name' => $project->info['name'],
            'drupal' => t('In core'),
            'backdrop' => $backdrop_status,
            'rec' => t($recommendation, array(':module' => $project->info['name'])),
          ),
          'class' => isset($class) ? array($class) : array(),
        );
        $core_module_rows[] = $row;
      }

      // Contrib modules.
      elseif (isset($project->info['version'])) {
        if (isset($project->info['project']) && $project->info['project'] != $last_project) {
          $drupal_version = $project->info['version'];
          $backdrop_version = '';
          $recommendation = '';
          $class = '';
          if ($available && array_key_exists($project->name, $backdrop_data)) {
            switch ($backdrop_data[$project->name]['status']) {
              case BACKDROP_UPGRADE_STATUS_CORE:
                $backdrop_version = '';
                if (isset($backdrop_data[$project->name]['in_core_note'])) {
                  $backdrop_version .= $backdrop_data[$project->name]['in_core_note'] . '<br>';
                }
                if (isset($backdrop_data[$project->name]['in_core_warning'])) {
                  $backdrop_version .= $backdrop_data[$project->name]['in_core_warning'];
                }
                $status = '';
                if (isset($backdrop_data[$project->name]['in_core_upgrade_status'])) {
                  $status = $backdrop_data[$project->name]['in_core_upgrade_status'];
                }
                $rec_translated = '';
                if (isset($backdrop_data[$project->name]['in_core_upgrade'])) {
                  $rec_translated = $backdrop_data[$project->name]['in_core_upgrade'];
                }
                break;
              case BACKDROP_UPGRADE_STATUS_STABLE;
                $backdrop_version = t('Stable :bversion', array(':bversion' => $backdrop_data[$project->name]['latest_version']));
                $status = t('Leave enabled.');
                $recommendation = 'Before upgrade: Add the stable contrib version of :module to Backdrop.';
                break;
              case BACKDROP_UPGRADE_STATUS_DEVELOPMENT:
                $backdrop_version = t('In Development :bversion', array(':bversion' => $backdrop_data[$project->name]['latest_version']));
                $recommendation = 'Before upgrade: Add the development contrib version of :module to Backdrop.';
                $class = 'warning';
                break;
              case UPDATE_UNKNOWN:
                // fall-through intentional.
              default:
                $backdrop_version = t('Unknown');
                $status = t('Leave enabled.');
                $recommendation = 'Port :module to Backdrop.';
                $class = 'error';
                break;
            }
          }

          $note = '';
          if (isset($status)) {
            $note .= '<strong>' . $status . '</strong><br>';
          }
          if (isset($recommendation)) {
            $note .=  t($recommendation, array(':module' => $project->info['name']));
          }
          elseif (isset($rec_translated))  {
            $node .= $rec_translated;
          }

          $row = array(
            'data' => array(
              'name' => $project->info['name'],
              'drupal' => $drupal_version,
              'backdrop' => $backdrop_version,
              'note' => $note,
            ),
            'class' => isset($class) ? array($class) : array(),
          );
          $contrib_module_rows[] = $row;
        }
      }

      // Custom modules.
      else {
        $custom_modules[$project->name] = $project;
        $row = array(
          'data' => array(
            'name' => $project->info['name'],
            'drupal' => t('Custom?'),
            'backdrop' => t('Unknown'),
            'rec' => t('Port :module to Backdrop.', array(':module' => $project->info['name'])),
          ),
          'class' => array('error'),
        );
        $custom_module_rows[] = $row;
      }

      // Save the last project so we don't include sub-modules in this list.
      if (isset($project->info['project'])) {
        $last_project = $project->info['project'];
      }
      else {
        $last_project = $project->name;
      }
    }
  }

  $core_module_data = array(
    'title' => t('Enabled Core Modules'),
    'info' => t('This is a summary of the enabled core modules.'),
    'header' => array(t('Module name'), t('Drupal status'), t('Backdrop status'), t('Recommendation')),
    'rows' => $core_module_rows,
    'collapsed' => $collapsed,
  );
  $renderable['core_modules'] = backdrop_upgrade_status_report_data($core_module_data);


  if ($available) {
    $contrib_module_data = array(
      'title' => t('Enabled Contrib Modules'),
      'info' => t('This is a summary of the enabled contributed modules.'),
      'header' => array(t('Module name'), t('Drupal version'), t('Backdrop status'), t('Recommendation')),
      'rows' => $contrib_module_rows,
      'collapsed' => $collapsed,
    );
    $renderable['contrib_modules'] = backdrop_upgrade_status_report_data($contrib_module_data);
  }

  $custom_module_data = array(
    'title' => t('Enabled Custom Modules'),
    'info' => t('This is a summary of the enabled custom modules.'),
    'header' => array(t('Module name'), t('Drupal status'), t('Backdrop status'), t('Recommendation')),
    'rows' => $custom_module_rows,
    'collapsed' => FALSE,
  );
  $renderable['custom_modules'] = backdrop_upgrade_status_report_data($custom_module_data);

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
  $user_data = array(
    'title' => t('User account data'),
    'info' => t('This is a summary of the user accounts on the site.'),
    'header' => array(t('User accounts'), t('Status'), t('Number of users')),
    'rows' => $rows,
  );
  $renderable['users'] = backdrop_upgrade_status_report_data($user_data);

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
  $node_data = array(
    'title' => t('Content type data'),
    'info' => t('Any conent types with zero itesm could be deleted.'),
    'header' => array(t('Conent type'), t('Machine name'), t('Number of items'), t('Recommendation')),
    'rows' => $rows,
  );
  $renderable['nodes'] = backdrop_upgrade_status_report_data($node_data);

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
        $count = db_query("SELECT count(entity_id) FROM {$table}")->fetchField();
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
  if (empty($rows)) {
    $output = t('No fields have been defined yet.');
  }

  $field_data = array(
    'title' => t('Field data'),
    'info' => t('Any fields with zero values could be removed.'),
    'header' => array(t('Field name'), t('Field type'), t('Module'), t('Used in'), t('Number of values'), t('Recommendation')),
    'rows' => $rows,
  );
  $renderable['fields'] = backdrop_upgrade_status_report_data($field_data);


  // Allow other modules to add things to the reports page.
  drupal_alter('backdrop_upgrade_status_report', $renderable);

  return system_settings_form($renderable);
}

/**
 * Creates a fieldset with tabular data for the reports page.
 *
 * @param array $variables
 *   Contains the following keys:
 *   - title
 *   - info
 *   - header
 *   - data
 *
 * @return array
 *   Renderable fieldset.
 */
function backdrop_upgrade_status_report_data($variables) {
  $title = $variables['title']; // has t().
  $info = $variables['info']; // has t().
  $header = $variables['header'];
  $rows = $variables['rows'];
  $collapsed = isset($variables['collapsed'])? $variables['collapsed']: TRUE;

  $fieldset = array(
    '#type' => 'fieldset',
    '#title' => $title,
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
  );
  $fieldset['info'] = array(
    '#type' => 'markup',
    '#markup' => $info,
  );
  $fieldset['table'] = array(
    '#type' => 'markup',
    '#markup' => theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('class' => array('backdrop-upgrade-table')),
    )),
  );

  return $fieldset;
}

/**
 * Implements hook_backdrop_upgrade_status_report_alter() on behalf of
 * paragraphs module.
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
    'title' => t('Paragraph type data'),
    'info' => t('Any paragraph types with zero items could be deleted.'),
    'header' => array(t('Paragraph type'), t('Machine name'), t('Number of paragraphs')),
    'rows' => $rows,
  );
  $renderable['paragraphs'] = backdrop_upgrade_status_report_data($paragraph_data);
}

/**
 * Implements hook_backdrop_upgrade_status_report_alter() on behalf of
 * views module.
 */
function views_backdrop_upgrade_status_report_alter(&$renderable) {
  // Get all the taxonomy terms in the system.
  $rows = array();
  $collapsed = TRUE;
  $views = views_get_all_views();
  foreach ($views as $view) {
    switch ($view->type) {
      case t('Normal'):
      case t('Overridden'):
        $type = t('In database');
        break;
      default:
        $type = t('In code');
        break;
    }
    $row = array(
      'data' => array(
        'name' => $view->human_name,
        'machine' => $view->name,
        'state' => $type,
        'description' => $view->description,
        'recommendation' => '',
      ),
    );

    if ($type == t('In code')) {
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
    $rows[] = $row;
  }
  $view_data = array(
    'title' => t('Views data'),
    'info' => t('All views should show STORAGE STATE as <strong>In database</strong> in order to be properly upgraded into Backdrop CMS. Note: You will first need to make the <em>Save</em> button appear on the Edit View form. Click on any setting to open a modal, then click <em>Cancel</em> to close the modal. The <em>Save</em> button will be at the top right.'),
    'header' => array(t('View name'), t('Machine name'), t('Storage state'), t('Description'), t('Recommendation')),
    'rows' => $rows,
    'collapsed' => $collapsed,
  );
  $renderable['views'] = backdrop_upgrade_status_report_data($view_data);
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 *   @todo  none of this works ?!?
 */
function backdrop_upgrade_status_form_views_ui_edit_form_alter(&$form, &$form_state, $view, $display_id) {
  // Don't hide the Save button, so that people can save views into the DB.
  $key = array_search('js-hide', $form['actions']['#attributes']['class']);
  if ($key !== FALSE) {
    unset($form['actions']['#attributes']['class'][$key]);
  }

  // Add CSS to force the Save button to display.
  //$path = drupal_get_path('module', 'backdrop_upgrade_status');
  //$form['#attached']['css'][$path . '/css/views-fix.css'] = array();

  // Add JS to remove the js-hide class.
  //$form['#attached']['js'][] = $path . '/js/views-fix.js';
  //dpm($form['#attached']['js']);

  // Mark the view as changed, and invalidate the cache.
  $form_state['view']->changed = TRUE;
  views_invalidate_cache();
}

/**
 * Implements hook_backdrop_upgrade_status_report_alter() on behalf of
 * features module.
 */
function features_backdrop_upgrade_status_report_alter(&$renderable) {
  //user_access('administer features')
  $rows = array();
  $collapsed = TRUE;
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
      if (in_array('views_view', $feature->components)) {
        $row['class'] = array('warning');
        $row['data']['recommendation'] = t('Move these views back into the database.');
      }
      if (in_array('node', $feature->components)) {
        $collapsed = FALSE;
        $row['class'] = array('error');
        $row['data']['recommendation'] = t('Unfeaturize these content types.');
        if (module_exists('unfeatureize') && user_access('administer content types')) {
          $href = 'admin/config/development/unfeaturize';
          $row['data']['recommendation'] = l(t('Unfeaturize these content types.'), $href);
        }
      }
      $rows[] = $row;
    }
  }

  $replacements = array('!unfeatureized' => l(t('unfeatureized'), 'https://www.drupal.org/project/unfeaturize'));
  $feature_data = array(
    'title' => t('Features data'),
    'info' => t('All configuration in Features will also need to be in the database in order to be properly upgraded into Backdrop CMS. Features using the <strong>node</strong> component may need to be !unfeatureized.', $replacements),
    'header' => array(t('Name'), t('Machine name'), t('Storage state'), t('Components'), t('Description'), t('Recommendation')),
    'rows' => $rows,
    'collapsed' => $collapsed,
  );
  $renderable['features'] = backdrop_upgrade_status_report_data($feature_data);
}
