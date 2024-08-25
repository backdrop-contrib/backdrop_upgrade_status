<?php
/**
 * @file
 * All Drupal 7 core modules that are no longer part of Backdrop core.
 */

/**
 * Get list of core modules removed from Backdrop, and what to do about each.
 *
 * @return array
 *   List of modules removed from core as an array with the follwing keys:
 *   - class: Class to color-code the row of the modules table.
 *   - rec: Recommendation for how to proceed.
 */
function _backdrop_upgrade_status_removed_core_modules() {
  $drupal_url = 'https://drupal.org/project/';
  // @todo - remove t() here and add instead at render point?
  return array(
    'aggregator' => array(
      'class' => 'warning',
      'rec' => t('Add the contrib version of !module module to Backdrop.',
        array('!module' => l('Aggregator'), 'https://backdropcms.org/project/aggregator')),
    ),
    'blog' => array(
      'class' => 'error',
      'rec' => t('Add the contrib version of !module module to Backdrop.',
        array('!module' => l('Blog'), 'https://backdropcms.org/project/blog')),
    ),
    'dashboard' => array(
      'class' => 'warning',
      'rec' => t('Alternative core module provided. Disable and uninstall the Drupal 7 version, then install on Backdrop after the upgrade.'),
    ),
    'forum' => array(
      'class' => 'error',
      'rec' => t('Add the contrib version of !module module to Backdrop.',
        array('!module' => l('Forum'), 'https://backdropcms.org/project/forum')),
    ),
    'help' => array(
      'class' => 'warning', // Warning, because help is rarely necessary.
      'rec' => t('Disable and Uninstall Help module, or port to Backdrop.)'),
    ),
    'openid' => array(
      'class' => 'error',
      'rec' => t('Disable and Uninstall OpenID module, or port to Backdrop.'),
    ),
    'overlay' => array(
      'class' => 'warning', // Warning, because overlay is rarely necessary.
      'rec' => t('Disable and Uninstall Overlay module, or port to Backdrop.'),
    ),
    'php' => array(
      'class' => 'error',
      'rec' => t('Review all content using the PHP code format. Delete the content, or change the format. Disable and Uninstall PHP Filter module.'),
    ),
    'poll' => array(
      'class' => 'error',
      'rec' => t('Add the contrib version of !module module to Backdrop.',
        array('!module' => l('Poll'), 'https://backdropcms.org/project/poll')),
    ),
    'profile' => array(
      'class' => 'error',
      'rec' => t('Add the contrib version of !module module to Backdrop.',
        array('!module' => l('Profile'), 'https://backdropcms.org/project/profile')),
    ),
    'rdf' => array(
      'class' => 'warning', // Warning, because rdf is rarely necessary.
      'rec' => t('Disable and Uninstall RDF module, or port to Backdrop.'),
    ),
    'shortcut' => array(
      'class' => 'warning', // Warning, because shortcut is rarely necessary.
      'rec' => t('Disable and Uninstall, or add the contrib version of !module module to Backdrop.',
        array('!module' => l('Shortcut'), 'https://backdropcms.org/project/shortcut')),
    ),
    'statistics' => array(
      'class' => 'warning', // Warning, because statistics is rarely necessary.
      'rec' => t('Disable and Uninstall, or add the contrib version of !module module to Backdrop.',
        array('!module' => l('Statistics'), 'https://backdropcms.org/project/statistics')),
    ),
    'toolbar' => array(
      'class' => '',
      'rec' => t('No action required. Alternative core module provided.'),
    ),
    'tracker' => array(
      'class' => 'warning', // Warning, because tracker is rarely necessary.
      'rec' => t('Disable and Uninstall Tracker module, or port to Backdrop.'),
    ),
    'trigger' => array(
      'class' => 'warning', // Warning, because trigger is rarely necessary.
      'rec' => t('Disable and Uninstall Trigger module, or port to Backdrop.'),
    ),
  );
}
