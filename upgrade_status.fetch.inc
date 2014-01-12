<?php

/**
 * @file
 * Code required only when fetching information about available updates.
 */

module_load_include('inc', 'update', 'update.fetch');

/**
 * Page callback: Checks for updates and displays the update status report.
 *
 * Manually checks the update status without the use of cron.
 *
 * @see upgrade_status_menu()
 */
function upgrade_status_manual_status() {
  _upgrade_status_refresh();
  $batch = array(
    'operations' => array(
      array('upgrade_status_fetch_data_batch', array()),
    ),
    'finished' => 'upgrade_status_fetch_data_finished',
    'title' => t('Checking available upgrade data'),
    'progress_message' => t('Trying to check available upgrade data ...'),
    'error_message' => t('Error checking available upgrade data.'),
    'file' => drupal_get_path('module', 'upgrade_status') . '/upgrade_status.fetch.inc',
  );
  batch_set($batch);
  batch_process('admin/reports/updates/upgrade');
}

/**
 * Batch callback: Processes a step in batch for fetching available update data.
 *
 * @param $context
 *   Reference to an array used for Batch API storage.
 */
function upgrade_status_fetch_data_batch(&$context) {
  $queue = DrupalQueue::get('upgrade_status_fetch_tasks');
  if (empty($context['sandbox']['max'])) {
    $context['finished'] = 0;
    $context['sandbox']['max'] = $queue->numberOfItems();
    $context['sandbox']['progress'] = 0;
    $context['message'] = t('Checking available upgrade data ...');
    $context['results']['updated'] = 0;
    $context['results']['failures'] = 0;
    $context['results']['processed'] = 0;
  }

  // Grab another item from the fetch queue.
  for ($i = 0; $i < 5; $i++) {
    if ($item = $queue->claimItem()) {
      if (_upgrade_status_process_fetch_task($item->data)) {
        $context['results']['updated']++;
        $context['message'] = t('Checked available upgrade data for %title.', array('%title' => $item->data['info']['name']));
      }
      else {
        $context['message'] = t('Failed to check available upgrade data for %title.', array('%title' => $item->data['info']['name']));
        $context['results']['failures']++;
      }
      $context['sandbox']['progress']++;
      $context['results']['processed']++;
      $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
      $queue->deleteItem($item);
    }
    else {
      // If the queue is currently empty, we're done. It's possible that
      // another thread might have added new fetch tasks while we were
      // processing this batch. In that case, the usual 'finished' math could
      // get confused, since we'd end up processing more tasks that we thought
      // we had when we started and initialized 'max' with numberOfItems(). By
      // forcing 'finished' to be exactly 1 here, we ensure that batch
      // processing is terminated.
      $context['finished'] = 1;
      return;
    }
  }
}

/**
 * Batch callback: Performs actions when all fetch tasks have been completed.
 *
 * @todo: I don't actually think we need to fork this; investigate later.
 *
 * @param $success
 *   TRUE if the batch operation was successful; FALSE if there were errors.
 * @param $results
 *   An associative array of results from the batch operation, including the key
 *   'updated' which holds the total number of projects we fetched available
 *   update data for.
 */
function upgrade_status_fetch_data_finished($success, $results) {
  if ($success) {
    if (!empty($results)) {
      if (!empty($results['updated'])) {
        drupal_set_message(format_plural($results['updated'], 'Checked available upgrade data for one project.', 'Checked available upgrade data for @count projects.'));
      }
      if (!empty($results['failures'])) {
        drupal_set_message(format_plural($results['failures'], 'Failed to get available upgrade data for one project.', 'Failed to get available upgrade data for @count projects.'), 'error');
      }
    }
  }
  else {
    drupal_set_message(t('An error occurred trying to get available upgrade data.'), 'error');
  }
}

/**
 * Attempts to drain the queue of tasks for release history data to fetch.
 */
function _upgrade_status_fetch_data() {
  $queue = DrupalQueue::get('upgrade_status_fetch_tasks');
  $end = time() + variable_get('update_max_fetch_time', UPDATE_MAX_FETCH_TIME);
  while (time() < $end && ($item = $queue->claimItem())) {
    _upgrade_status_process_fetch_task($item->data);
    $queue->deleteItem($item);
  }
}

/**
 * Processes a task to fetch available update data for a single project.
 *
 * Once the release history XML data is downloaded, it is parsed and saved into
 * the {cache_update} table in an entry just for that project.
 *
 * @param $project
 *   Associative array of information about the project to fetch data for.
 *
 * @return
 *   TRUE if we fetched parsable XML, otherwise FALSE.
 */
function _upgrade_status_process_fetch_task($project) {
  global $base_url;
  $fail = &drupal_static(__FUNCTION__, array());
  // This can be in the middle of a long-running batch, so REQUEST_TIME won't
  // necessarily be valid.
  $now = time();
  if (empty($fail)) {
    // If we have valid data about release history XML servers that we have
    // failed to fetch from on previous attempts, load that from the cache.
    if (($cache = _update_cache_get('upgrade_status_fetch_failures')) && ($cache->expire > $now)) {
      $fail = $cache->data;
    }
  }

  $max_fetch_attempts = variable_get('update_max_fetch_attempts', UPDATE_MAX_FETCH_ATTEMPTS);

  $success = FALSE;
  $available = array();
  // US: No site key to avoid hi-jacking module usage statistics.
#  $site_key = drupal_hmac_base64($base_url, drupal_get_private_key());
  // US: However, we want to check against a future core version.
  $version = variable_get('upgrade_status_core_version', UPGRADE_STATUS_CORE_VERSION);
  $url = _upgrade_status_build_fetch_url($project, $version);
  $fetch_url_base = _update_get_fetch_url_base($project);
  $project_name = $project['name'];

  if (empty($fail[$fetch_url_base]) || $fail[$fetch_url_base] < $max_fetch_attempts) {
    $xml = drupal_http_request($url);
    if (!isset($xml->error) && isset($xml->data)) {
      $data = $xml->data;
    }
  }

  if (!empty($data)) {
    $available = update_parse_xml($data);
    // @todo: Purge release data we don't need (http://drupal.org/node/238950).
    if (!empty($available)) {
      // Only if we fetched and parsed something sane do we return success.
      $success = TRUE;
    }
  }
  else {
    $available['project_status'] = 'not-fetched';
    if (empty($fail[$fetch_url_base])) {
      $fail[$fetch_url_base] = 1;
    }
    else {
      $fail[$fetch_url_base]++;
    }
  }

  $frequency = variable_get('update_check_frequency', 1);
  $cid = 'upgrade_status_available_releases::' . $project_name;
  _update_cache_set($cid, $available, $now + (60 * 60 * 24 * $frequency));

  // Stash the $fail data back in the DB for the next 5 minutes.
  _update_cache_set('upgrade_status_fetch_failures', $fail, $now + (60 * 5));

  // Whether this worked or not, we did just (try to) check for updates.
  variable_set('upgrade_status_last_check', $now);

  // Now that we processed the fetch task for this project, clear out the
  // record in {cache_update} for this task so we're willing to fetch again.
  _update_cache_clear('upgrade_status_fetch_task::' . $project_name);

  return $success;
}

/**
 * Clears out all the cached available update data and initiates re-fetching.
 */
function _upgrade_status_refresh() {
  module_load_include('inc', 'upgrade_status', 'upgrade_status.compare');

  // Since we're fetching new available update data, we want to clear
  // our cache of both the projects we care about, and the current update
  // status of the site. We do *not* want to clear the cache of available
  // releases just yet, since that data (even if it's stale) can be useful
  // during update_get_projects(); for example, to modules that implement
  // hook_system_info_alter() such as cvs_deploy.
  // @todo: File upstream issue: cvs_deploy is irrelevant. :P
  _update_cache_clear('upgrade_status_project_projects');
  _update_cache_clear('upgrade_status_project_data');

  $projects = update_get_projects();

  // Now that we have the list of projects, we should also clear our cache of
  // available release data, since even if we fail to fetch new data, we need
  // to clear out the stale data at this point.
  _update_cache_clear('upgrade_status_available_releases::', TRUE);

  foreach ($projects as $key => $project) {
    upgrade_status_create_fetch_task($project);
  }
}

/**
 * Adds a task to the queue for fetching release history data for a project.
 *
 * We only create a new fetch task if there's no task already in the queue for
 * this particular project (based on 'upgrade_status_fetch_task::' entries in the
 * {cache_update} table).
 *
 * @param $project
 *   Associative array of information about a project as created by
 *   update_get_projects(), including keys such as 'name' (short name), and the
 *   'info' array with data from a .info file for the project.
 *
 * @see update_get_projects()
 * @see update_get_available()
 * @see upgrade_status_refresh()
 * @see upgrade_status_fetch_data()
 * @see _upgrade_status_process_fetch_task()
 */
function _upgrade_status_create_fetch_task($project) {
  $fetch_tasks = &drupal_static(__FUNCTION__, array());
  if (empty($fetch_tasks)) {
    $fetch_tasks = _update_get_cache_multiple('upgrade_status_fetch_task');
  }
  $cid = 'upgrade_status_fetch_task::' . $project['name'];
  if (empty($fetch_tasks[$cid])) {
    $queue = DrupalQueue::get('upgrade_status_fetch_tasks');
    $queue->createItem($project);
    // Due to race conditions, it is possible that another process already
    // inserted a row into the {cache_update} table and the following query will
    // throw an exception.
    // @todo: Remove the need for the manual check by relying on a queue that
    // enforces unique items.
    try {
      db_insert('cache_update')
          ->fields(array(
            'cid' => $cid,
            'created' => REQUEST_TIME,
          ))
          ->execute();
    }
    catch (Exception $e) {
      // The exception can be ignored safely.
    }
    $fetch_tasks[$cid] = REQUEST_TIME;
  }
}

/**
 * Generates the URL to fetch information about project updates.
 *
 * This figures out the right URL to use, based on the project's .info file and
 * the global defaults. Appends optional query arguments when the site is
 * configured to report usage stats.
 *
 * @param $project
 *   The array of project information from update_get_projects().
 * // US: Replace $site_key with $version.
 * @param $version
 *   The target version of Drupal core you wish to query.
 *
 * @return
 *   The URL for fetching information about updates to the specified project.
 *
 * @see update_fetch_data()
 * @see _update_process_fetch_task()
 * @see update_get_projects()
 */
function _upgrade_status_build_fetch_url($project, $version) {
  $name = $project['name'];
  $url = _update_get_fetch_url_base($project);
  $url .= '/' . $name . '/' . $version;

  // US: Remove all handling around $site_key; we don't want to pollute project
  // usage statistics, so we don't use any of it.

  return $url;
}

/**
 * Returns the base of the URL to fetch available update data for a project.
 *
 * @param $project
 *   The array of project information from update_get_projects().
 *
 * @return
 *   The base of the URL used for fetching available update data. This does
 *   not include the path elements to specify a particular project, version,
 *   site_key, etc.
 *
 * @see _update_build_fetch_url()
 */
function _upgrade_status_get_fetch_url_base($project) {
  return isset($project['info']['project status url']) ? $project['info']['project status url'] : variable_get('update_fetch_url', UPDATE_DEFAULT_URL);
}

/**
 * Performs any notifications that should be done once cron fetches new data.
 *
 * This method checks the status of the site using the new data and, depending
 * on the configuration of the site, notifies administrators via e-mail if there
 * are new releases or missing security updates.
 *
 * @see update_requirements()
 */
function _upgrade_status_cron_notify() {
  module_load_install('update');
  $status = update_requirements('runtime');
  $params = array();
  $notify_all = (variable_get('update_notification_threshold', 'all') == 'all');
  foreach (array('core', 'contrib') as $report_type) {
    $type = 'update_' . $report_type;
    if (isset($status[$type]['severity'])
        && ($status[$type]['severity'] == REQUIREMENT_ERROR || ($notify_all && $status[$type]['reason'] == UPDATE_NOT_CURRENT))) {
      $params[$report_type] = $status[$type]['reason'];
    }
  }
  if (!empty($params)) {
    $notify_list = variable_get('update_notify_emails', '');
    if (!empty($notify_list)) {
      $default_language = language_default();
      foreach ($notify_list as $target) {
        if ($target_user = user_load_by_mail($target)) {
          $target_language = user_preferred_language($target_user);
        }
        else {
          $target_language = $default_language;
        }
        $message = drupal_mail('update', 'status_notify', $target, $target_language, $params);
        // Track when the last mail was successfully sent to avoid sending
        // too many e-mails.
        if ($message['result']) {
          variable_set('update_last_email_notification', REQUEST_TIME);
        }
      }
    }
  }
}

/**
 * Parses the XML of the Drupal release history info files.
 *
 * @param $raw_xml
 *   A raw XML string of available release data for a given project.
 *
 * @return
 *   Array of parsed data about releases for a given project, or NULL if there
 *   was an error parsing the string.
 */
function upgrade_status_parse_xml($raw_xml) {
  try {
    $xml = new SimpleXMLElement($raw_xml);
  }
  catch (Exception $e) {
    // SimpleXMLElement::__construct produces an E_WARNING error message for
    // each error found in the XML data and throws an exception if errors
    // were detected. Catch any exception and return failure (NULL).
    return;
  }
  // If there is no valid project data, the XML is invalid, so return failure.
  if (!isset($xml->short_name)) {
    return;
  }
  $short_name = (string) $xml->short_name;
  $data = array();
  foreach ($xml as $k => $v) {
    $data[$k] = (string) $v;
  }
  $data['releases'] = array();
  if (isset($xml->releases)) {
    foreach ($xml->releases->children() as $release) {
      $version = (string) $release->version;
      $data['releases'][$version] = array();
      foreach ($release->children() as $k => $v) {
        $data['releases'][$version][$k] = (string) $v;
      }
      $data['releases'][$version]['terms'] = array();
      if ($release->terms) {
        foreach ($release->terms->children() as $term) {
          if (!isset($data['releases'][$version]['terms'][(string) $term->name])) {
            $data['releases'][$version]['terms'][(string) $term->name] = array();
          }
          $data['releases'][$version]['terms'][(string) $term->name][] = (string) $term->value;
        }
      }
    }
  }
  return $data;
}