<?php
// $Id$

/**
 * @file
 * Checks to see if your installed modules are available for the next major
 * release of Drupal.
 */

/**
 * Default version of core we want to query for.
 */
define('UPGRADE_STATUS_CORE_VERSION', '7.x');

/**
 * URL to check updates at, if a given project doesn't define its own.
 */
define('UPGRADE_STATUS_DEFAULT_URL', 'http://updates.drupal.org/release-history');

/**
 * Project has not been ported yet.
 */
define('UPGRADE_STATUS_NOT_PORTED', 3);

/**
 * Project has a new release available, but it is not a security release.
 */
define('UPGRADE_STATUS_DEVELOPMENT', 4);

/**
 * Project is available.
 */
define('UPGRADE_STATUS_STABLE', 5);

/**
 * Project has been moved into core.
 */
define('UPGRADE_STATUS_CORE', 6);


/**
 * Menu callback. Generate a page about the upgrade status of projects.
 */
function upgrade_status_status() {
  if ($available = upgrade_status_get_available()) {
    $data = upgrade_status_calculate_project_data($available);
    return theme('upgrade_status_report', $data);
  }
  else {
    return theme('upgrade_status_report', t('No information is available about potential new releases for currently installed modules. To check for updates, you can <a href="@check_manually">check manually</a>. Please note that checking for available updates can take a long time, so please be patient.', array(
      '@check_manually' => url('admin/reports/updates/check', array('query' => array('query' => $destination))),
    )));
  }
}

/**
 * Menu callback to manually check the upgrade status.
 */
function upgrade_status_manual_status() {
  if (upgrade_status_refresh()) {
    drupal_set_message(t('Fetched information about all available upgrades.'));
  }
  else {
    drupal_set_message(t('Unable to fetch any information on available upgrades.'), 'error');
  }
  drupal_goto('admin/reports/updates/upgrade');
}

/**
 * Given the installed projects and the available release data retrieved from
 * remote servers, calculate the current status.
 *
 * This function is the heart of the update status feature. It iterates over
 * every currently installed project. For each one, it first checks if the
 * project has been flagged with a special status like "unsupported" or
 * "insecure", or if the project node itself has been unpublished. In any of
 * those cases, the project is marked with an error and the next project is
 * considered.
 *
 * @param $available
 *  Array of data about available project releases.
 *
 * @see upgrade_status_get_available()
 * @see update_status_get_projects()
 * @see update_status_process_project_info()
 */
function upgrade_status_calculate_project_data($available) {

  // Retrieve the projects from cache, if present.
  $projects = upgrade_status_project_cache('upgrade_status_data');
  // If $projects is empty, then the cache must be rebuilt.
  // Otherwise, return the cached data and skip the rest of the function.
  if (!empty($projects)) {
    return $projects;
  }

  module_load_include('inc', 'update', 'update.compare');
  $projects = update_get_projects();
  update_process_project_info($projects);
  $settings = variable_get('upgrade_status_settings', array());
  foreach ($projects as $project => $project_info) {
    if (isset($available[$project])) {
      // If the project status is marked as something bad, there's nothing
      // else to consider.
      if (isset($available[$project]['project_status'])) {
        switch ($available[$project]['project_status']) {
          case 'unpublished':
          case 'revoked':
          case 'unsupported':
            $projects[$project]['status'] = UPDATE_STATUS_NOT_SUPPORTED;
            if (empty($projects[$project]['extra'])) {
              $projects[$project]['extra'] = array();
            }
            $projects[$project]['extra'][] = array(
              'class' => 'project-not-supported',
              'label' => t('Project not supported'),
              'data' => t('This project is no longer supported, and is no longer available for download. Disabling everything included by this project is strongly recommended!'),
            );
            break;
        }
      }

      if (!empty($projects[$project]['status'])) {
        // We already know the status for this project, so there's nothing
        // else to compute. Just record everything else we fetched from the
        // XML file into our projects array and move to the next project.
        $projects[$project] += $available[$project];
        continue;
      }

      // Figure out the target major version.
      $existing_major = $project_info['existing_major'];
      $supported_majors = array();
      if (isset($available[$project]['supported_majors'])) {
        $supported_majors = explode(',', $available[$project]['supported_majors']);
      }

      if (isset($available[$project]['recommended_major'])) {
        $target_major = $available[$project]['recommended_major'];
      }
      elseif (isset($available[$project]['default_major'])) {
        // Older release history XML file without recommended, so recommend
        // the currently defined "default_major" version.
        $target_major = $available[$project]['default_major'];
      }
      else {
        // Malformed XML file? Stick with the current version.
        $target_major = $existing_major;
      }

      $version_patch_changed = '';
      $patch = '';

      // Defend ourselves from XML history files that contain no releases.
      if (empty($available[$project]['releases'])) {
        $projects[$project]['status'] = UPDATE_STATUS_UNKNOWN;
        $projects[$project]['reason'] = t('No available releases found');
        continue;
      }
      foreach ($available[$project]['releases'] as $version => $release) {
        // Otherwise, ignore unpublished, insecure, or unsupported releases.
        if ($release['status'] == 'unpublished' || 
            (isset($release['terms']['Release type']) &&
             (in_array('Insecure', $release['terms']['Release type']) ||
              in_array('Unsupported', $release['terms']['Release type'])))) {
          continue;
        }

        // See if this is a higher major version than our target and yet still
        // supported. If so, record it as an "Also available" release.
        if ($release['version_major'] > $target_major) {
          if (in_array($release['version_major'], $supported_majors)) {
            if (!isset($available[$project]['also'])) {
              $available[$project]['also'] = array();
            }
            if (!isset($available[$project]['also'][$release['version_major']])) {
              $available[$project]['also'][$release['version_major']] = $version;
            }
          }
          // Otherwise, this release can't matter to us, since it's neither
          // from the release series we're currently using nor the recommended
          // release. We don't even care about security updates for this
          // branch, since if a project maintainer puts out a security release
          // at a higher major version and not at the lower major version,
          // they must change the default major release at the same time, in
          // which case we won't hit this code.
          continue;
        }

        // Look for the 'latest version' if we haven't found it yet. Latest is
        // defined as the most recent version for the target major version.
        if (!isset($available[$project]['latest_version'])
            && $release['version_major'] == $target_major) {
          $available[$project]['latest_version'] = $version;
        }

        // Look for the development snapshot release for this branch.
        if (!isset($available[$project]['dev_version'])
            && isset($release['version_extra'])
            && $release['version_extra'] == 'dev') {
          $available[$project]['dev_version'] = $version;
        }

        // Look for the 'recommended' version if we haven't found it yet (see
        // phpdoc at the top of this function for the definition).
        if (!isset($available[$project]['recommended'])
            && $release['version_major'] == $target_major
            && isset($release['version_patch'])) {
          if ($patch != $release['version_patch']) {
            $patch = $release['version_patch'];
            $version_patch_changed = $release['version'];
          }
          if (empty($release['version_extra']) && $patch == $release['version_patch']) { 
            $available[$project]['recommended'] = $version_patch_changed;
          }
        }
      }

      // If we were unable to find a recommended version, then make the latest
      // version the recommended version if possible.
      if (!isset($available[$project]['recommended']) && isset($available[$project]['latest_version'])) {
        $available[$project]['recommended'] = $available[$project]['latest_version'];
        $projects[$project]['status'] = UPGRADE_STATUS_DEVELOPMENT;
        $projects[$project]['reason'] = t('In development');
      }

      // Stash the info about available releases into our $projects array.
      $projects[$project] += $available[$project];

      if (isset($projects[$project]['status'])) {
        // If we already know the status, we're done.
        continue;
      }

      // If we don't know what to recommend, there's nothing we can report.
      // Bail out early.
      if (!isset($projects[$project]['recommended'])) {
        $projects[$project]['status'] = UPDATE_STATUS_UNKNOWN;
        $projects[$project]['reason'] = t('No available releases found');
        continue;
      }

      // Figure out the status, based on what we've seen and the install type.
      // Note: If we were not yet able to assign a status, this project already
      // provides a stable release.
      switch ($projects[$project]['type']) {
        case 'official':
        case 'dev':
          $projects[$project]['status'] = UPGRADE_STATUS_STABLE;
          $projects[$project]['reason'] = t('Available');
          break;

        default:
          $projects[$project]['status'] = UPDATE_STATUS_UNKNOWN;
          $projects[$project]['reason'] = t('Invalid info');
      }
    }
    elseif (!upgrade_status_moved_into_core($projects, $project)) {
      $projects[$project]['status'] = UPGRADE_STATUS_NOT_PORTED;
      $projects[$project]['reason'] = t('Not ported yet');
    }
  }
  cache_set('upgrade_status_data', $projects, 'cache', time() + (60 * 60));
  return $projects;
}

/**
 * Return status and notice about modules moved into Core.
 *
 * Assign custom upgrade information for certain modules.
 *
 * @param $projects
 *   Array of projects from upgrade_status_calculate_project_data().
 * @param $project
 *   Project name to check.
 * @return
 *   TRUE if module has been moved into core.
 */
function upgrade_status_moved_into_core(&$projects, $project) {
  $core = TRUE;
  switch ($project) {
    case 'actions':
      $projects[$project]['in_core_since'] = '6.x';
      $projects[$project]['in_core_note'] = t('Please note that the syntax for actions used by the 5.x-1.x and 4.7 versions of Actions module are different to triggers in Drupal 6.x. For further information please refer to the <a href="!project-url">Actions</a> module project page.', array('!project-url' => 'http://drupal.org/project/actions'));
      break;

    case 'ahah_forms':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'autolocale':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'book_bridge':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'find_path':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'html_to_text':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'htmlcorrector':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'javascript_aggregator':
      $projects[$project]['in_core_since'] = '6.x';
      $projects[$project]['in_core_note'] = t('Please note that there is a version of <a href="!project-url">JavaScript Aggregator</a> module for Drupal 6.x, which additionally minifies JavaScript.', array('!project-url' => 'http://drupal.org/project/javascript_aggregator'));
      break;

    case 'openid':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'themesettingsapi':
      $projects[$project]['in_core_since'] = '6.x';
      break;

    case 'update_status':
      $projects[$project]['in_core_since'] = '6.x';
      $projects[$project]['in_core_note'] = t('Please note that some of the advanced settings in the 5.x version of Update status are not present in the update.module in 6.x core, and have been moved into the <a href="!project-url">Update status advanced settings</a> module for Drupal 6.x and beyond.', array('!project-url' => 'http://drupal.org/project/update_advanced'));
      break;

    case 'user_status':
      $projects[$project]['in_core_since'] = '6.x';
      $projects[$project]['in_core_note'] = t('There is no database upgrade path for sites that used the 5.x version of the user_status module to migrate the message templates to the new settings in core. Furthermore, the place-holders available in these templates are different in 6.x core. Therefore, users will have to re-enter their message templates into the core settings at admin/user/settings.');
      break;

    default:
      $core = FALSE;
  }
  if ($core) {
    $projects[$project]['status'] = UPGRADE_STATUS_CORE;
  }
  return $core;
}

/**
 * Theme project status report.
 */
function theme_upgrade_status_report($data) {
  $last = variable_get('upgrade_status_last', 0);
  $current_data = update_get_available(TRUE);

  $output = '<p>'. t("Clicking on any of the modules' boxes will expand the area and show you a link to download the new version of the project, as well as read its release notes.") .'</p>';
  $output .= '<div class="upgrade-status checked"><p>'. t('Last checked: ') . ($last ? format_interval(time() - $last) .' '. t('ago') : t('Never'));
  $output .= ' <span class="check-manually">('. l(t('Check manually'), 'admin/reports/updates/upgrade/check') .')</span>';
  $output .= "</p></div>\n";

  $output .= drupal_get_form('upgrade_status_core_version_form');

  if (!is_array($data)) {
    $output .= '<p>'. $data .'</p>';
    return $output;
  }

  // Move 'drupal' to the top.
  $data = array('drupal' => $data['drupal']) + $data;

  $header = array();
  $rows = array();

  foreach ($data as $project) {
    // Skip upgrade_status. Doesn't make sense to display it.
    if ($project['name'] == 'upgrade_status') {
      continue;
    }
    switch ($project['status']) {
      case UPGRADE_STATUS_STABLE:
        $class = 'ok';
        $icon = theme('image', 'misc/watchdog-ok.png', t('ok'), t('ok'));
        break;

      case UPDATE_STATUS_REVOKED:
      case UPGRADE_STATUS_NOT_PORTED:
        $class = 'error';
        $icon = theme('image', 'misc/watchdog-error.png', t('error'), t('error'));
        break;

      case UPGRADE_STATUS_DEVELOPMENT:
      default:
        $class = 'warning';
        $icon = theme('image', 'misc/watchdog-warning.png', t('warning'), t('warning'));
        break;
    }
    // Special handling for project moved into core.
    if ($project['status'] == UPGRADE_STATUS_CORE) {
      $class = 'ok';
      if (!empty($project['in_core_note'])) {
        $icon = theme('image', 'misc/watchdog-warning.png', t('warning'), t('warning'));
      }
      else {
        $icon = theme('image', 'misc/watchdog-ok.png', t('ok'), t('ok'));
      }
    }

    // Compact layout. 31/05/2008 sun
    $row = '<div class="project">';
    $row .= '<div class="version-status">';
    switch ($project['status']) {
      case UPDATE_STATUS_REVOKED:
        $row .= '<span class="revoked">'. t('Revoked!') .'</span>';
        break;
      case UPGRADE_STATUS_NOT_PORTED:
        $row .= '<span class="not-supported">'. t('Not ported yet') .'</span>';
        break;
      case UPGRADE_STATUS_DEVELOPMENT:
        $row .= '<span class="not-current">'. t('In development') .'</span>';
        break;
      case UPGRADE_STATUS_STABLE:
      case UPGRADE_STATUS_CORE:
        $row .= '<span class="current">'. t('Available') .'</span>';
        break;
      default:
        $row .= check_plain($project['reason']);
        break;
    }

    $row .= '<span class="icon">'. $icon .'</span>';
    $row .= "</div>\n";

    // Collapse-trigger icon.
    $row .= theme('image', 'misc/menu-collapsed.png', '', '', array('class' => 'collapse-icon'));

    $row .= '<span class="project-title">';
    if (isset($project['title'])) {
      if (isset($project['link'])) {
        $row .= l($project['title'], $project['link']);
      }
      else {
        $row .= check_plain($project['title']);
      }
    }
    // Couldn't find this project's data for the next version of Drupal core.
    // Let's try the current one instead.
    elseif (isset($current_data[$project['name']]) && isset($current_data[$project['name']]['title'])) {
      if (isset($current_data[$project['name']]['link'])) {
        $row .= l($current_data[$project['name']]['title'], $current_data[$project['name']]['link']);
      }
      else {
        $row .= check_plain($current_data[$project_name]['title']);
      }
    }
    // Otherwise, just print the name.
    else {
      $row .= check_plain($project['name']);
    }
    $row .= '</span>';
    $row .= '<span class="existing-version">';
    $row .= ' '. check_plain($project['existing_version']);
    if ($project['type'] == 'dev' && !empty($project['datestamp'])) {
      $row .= ' <span class="version-date">('. format_date($project['datestamp'], 'custom', 'Y-M-d') .')</span>';
    }
    $row .= '</span>';

    // Compact layout. 31/05/2008 sun
    $row .= "<div class=\"details-wrapper\">\n";
    if ($project['status'] == UPGRADE_STATUS_CORE) {
      $row .= "<div class=\"core-notice\">\n";
      $row .= '<p>'. t('In Drupal core since @version', array('@version' => $project['in_core_since'])) .'</p>';
      $row .= '<p>'. $project['in_core_note'] .'</p>';
      $row .= '</div>';
    }
    $row .= "<div class=\"versions\">\n";

    if (isset($project['recommended'])) {
      if ($project['status'] != UPDATE_STATUS_CURRENT || $project['existing_version'] != $project['recommended']) {

        // First, figure out what to recommend.
        // If there's only 1 security update and it has the same version we're
        // recommending, give it the same CSS class as if it was recommended,
        // but don't print out a separate "Recommended" line for this project.
        if (!empty($project['security updates']) && count($project['security updates']) == 1 && $project['security updates'][0]['version'] == $project['recommended']) {
          $security_class = ' version-recommended version-recommended-strong';
        }
        else {
          $security_class = '';
          $version_class = 'version-recommended';
          // Apply an extra class if we're displaying both a recommended
          // version and anything else for an extra visual hint.
          if ($project['recommended'] != $project['latest_version']
              || !empty($project['also'])
              || ($project['type'] == 'dev'
                 && isset($project['dev_version'])
                 && $project['latest_version'] != $project['dev_version']
                 && $project['recommended'] != $project['dev_version'])
              || (isset($project['security updates'][0])
                 && $project['recommended'] != $project['security updates'][0])
              ) { 
            $version_class .= ' version-recommended-strong';
          }
          $row .= theme('upgrade_status_version', $project['releases'][$project['recommended']], t('Recommended version:'), $version_class);
        }

        // Now, print any security updates.
        if (!empty($project['security updates'])) {
          foreach ($project['security updates'] as $security_update) {
            $row .= theme('upgrade_status_version', $security_update, t('Security update:'), 'version-security'. $security_class);
          }
        }
      }

      if ($project['recommended'] != $project['latest_version']) {
        $row .= theme('upgrade_status_version', $project['releases'][$project['latest_version']], t('Latest version:'), 'version-latest');
      }
      if ($project['type'] == 'dev'
          && $project['status'] != UPDATE_STATUS_CURRENT
          && isset($project['dev_version'])
          && $project['recommended'] != $project['dev_version']) {
        $row .= theme('upgrade_status_version', $project['releases'][$project['dev_version']], t('Development version:'), 'version-latest');
      }
    }

    if (isset($project['also'])) {
      foreach ($project['also'] as $also) {
        $row .= theme('upgrade_status_version', $project['releases'][$also], t('Also available:'), 'version-also-available');
      }        
    }

    $row .= "</div>\n"; // versions div.

    $row .= "<div class=\"info\">\n";
    if (!empty($project['notes'])) {
      $row .= '<div class="notes">';
      $row .= t('Administrator note: %notes', array('%notes' => $project['notes']));
      $row .= "</div>\n";
    }

    $row .= '<div class="modules">';
    $row .= t('Includes: %modules', array('%modules' => implode(', ', $project['includes'])));
    $row .= "</div>\n";

    $row .= "</div>\n"; // info div.
    $row .= "</div>\n"; // wrapper div.
    $row .= "</div>\n"; // project div.

    $rows[] = array(
      'class' => $class,
      'data' => array($row),
    );
  }

  $output .= theme('table', $header, $rows, array('class' => 'upgrade-status'));
  drupal_add_css(drupal_get_path('module', 'upgrade_status') .'/upgrade_status.css');
  drupal_add_js(drupal_get_path('module', 'upgrade_status') .'/upgrade_status.js');
  return $output;
}

function theme_upgrade_status_version($version, $tag, $class) {
  $output = '';
  $output .= '<table class="version '. $class .'">';
  $output .= '<tr>';
  $output .= '<td class="version-title">'. $tag ."</td>\n";
  $output .= '<td class="version-details">';
  $output .= l($version['version'], $version['release_link']);
  $output .= ' <span class="version-date">('. format_date($version['date'], 'custom', 'Y-M-d') .')</span>';
  $output .= "</td>\n";
  $output .= '<td class="version-links">';
  $output .= l(t('Download'), $version['download_link']) .' · '. l(t('Release notes'), $version['release_link']);
  $output .= '</td>';
  $output .= '</tr>';
  $output .= "</table>\n";
  return $output;
}

/**
 * Fetch project info via XML from a central server.
 */
function upgrade_status_refresh() {
  global $base_url;

  @set_time_limit(240);

  // Since we're fetching new available update data, we want to clear
  // everything in our cache, to ensure we recompute the status. Note that
  // this does not cause update_status_get_projects() to be recomputed twice
  // in the same page load (e.g. when manually checking) since that function
  // stashes its answer in a static array.
  upgrade_status_invalidate_cache();

  $available = array();
  $data = array();
  $version = variable_get('upgrade_status_core_version', UPGRADE_STATUS_CORE_VERSION);

  module_load_include('inc', 'update', 'update.compare');
  $projects = update_get_projects();
  foreach ($projects as $key => $project) {
    // No site key to avoid hi-jacking module usage statistics.
    $url = _upgrade_status_build_fetch_url($project, $version);
    $xml = drupal_http_request($url);
    if (isset($xml->data)) {
      $data[] = $xml->data;
    }
  }

  if ($data) {
    module_load_include('inc', 'update', 'update.fetch');
    $parser = new update_xml_parser;
    $available = $parser->parse($data);
    cache_set('upgrade_status_info', $available);
    variable_set('upgrade_status_last', time());
    watchdog('upgrade_status', 'Fetched information about all available new releases and updates.', array(), WATCHDOG_NOTICE, l('view', 'admin/reports/updates'));
  }
  else {
    watchdog('upgrade_status', 'Unable to fetch any information on available new releases and updates.', array(), WATCHDOG_ERROR, l('view', 'admin/reports/updates'));
  }
  return $available;
}

/**
 * Generates the URL to fetch information about project updates.
 *
 * This figures out the right URL to use, based on the project's .info file
 * and the global defaults. Appends optional query arguments when the site is
 * configured to report usage stats.
 *
 * @param $project
 *   The array of project information from update_status_get_projects().
 * @param $version
 *   The target version of Drupal core you wish to query.
 *
 * @see upgrade_status_refresh()
 * @see update_status_get_projects()
 */
function _upgrade_status_build_fetch_url($project, $version) {
  $default_url = variable_get('upgrade_status_fetch_url', UPGRADE_STATUS_DEFAULT_URL);
  if (!isset($project['info']['project status url'])) {
    $project['info']['project status url'] = $default_url;
  }
  $name = $project['name'];
  $url = $project['info']['project status url'];
  $url .= '/'. $name .'/'. $version;
  return $url;
}

/**
 * Internal helper to try to get the update information from the cache
 * if possible, and to refresh the cache when necessary.
 *
 * @param $refresh
 *   Boolean to indicate if this method should refresh the cache automatically
 *   if there's no data.
 */
function upgrade_status_get_available($refresh = FALSE) {
  $available = array();
  if (!$refresh && $cache = cache_get('upgrade_status_info', 'cache')) {
    $available = $cache->data;
  }
  else {
    $available = upgrade_status_refresh();
  }
  return $available;
}

/**
 * Invalidates any cached data relating to update status.
 */
function upgrade_status_invalidate_cache() {
  cache_clear_all('upgrade_status_', 'cache', TRUE);
}

/**
 * Retrieve data from {cache} or empty the cache when necessary.
 *
 * Two very expensive arrays computed by this module are the list of all
 * installed modules (and .info data, project associations, etc), and the
 * current status of the site relative to the currently available
 * releases. These two arrays are cached in the {cache} table and used
 * whenever possible. The cache is cleared whenever the administrator visits
 * the status report, available updates report, or the module administration
 * pages, since we should always recompute the most current values on any of
 * those pages.
 *
 * @param $cid
 *   The cache id of data to return from the cache. Valid options are
 *   'upgrade_status_data' and 'update_status_projects'.
 *
 * @return
 *   The cached value of the $projects array generated by
 *   upgrade_status_calculate_project_data() or update_status_get_projects(),
 *   or an empty array when the cache is cleared.
 */
function upgrade_status_project_cache($cid) {
  $projects = array();
  if ($cache = cache_get($cid, 'cache')) {
    if (!empty($cache->data) && $cache->expire > time()) {
      $projects = $cache->data;
    }
  }
  return $projects;
}

/**
 * Form to display Drupal core version selection. 
 */
function upgrade_status_core_version_form(&$form_state) {
  $form['upgrade_status_core_version'] = array(
    '#type' => 'select',
    '#title' => t('Target version of Drupal core'),
    '#options' => drupal_map_assoc(array('7.x', '8.x')),
    '#default_value' => variable_get('upgrade_status_core_version', UPGRADE_STATUS_CORE_VERSION),
    '#description' => t('Select the version of Drupal core you wish to check for project status.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change'),
  );
  return $form;
}

/**
 * Set the new Drupal core version in a variable; refresh project data.
 */
function upgrade_status_core_version_form_submit($form, &$form_state) {
  // Refresh status if a different version than the default has been selected.
  if ($form_state['values']['upgrade_status_core_version'] != UPGRADE_STATUS_CORE_VERSION) {
    variable_set('upgrade_status_core_version', $form_state['values']['upgrade_status_core_version']);
    upgrade_status_refresh();
  }
  // Refresh status if the default version has been selected, but only if it
  // was not selected before (noob protection).
  else if (variable_get('upgrade_status_core_version', UPGRADE_STATUS_CORE_VERSION) != UPGRADE_STATUS_CORE_VERSION) {
    variable_del('upgrade_status_core_version');
    upgrade_status_refresh();
  }
  // ...or just ensure that we've got no stale variable set.
  else {
    variable_del('upgrade_status_core_version');
  }
}

