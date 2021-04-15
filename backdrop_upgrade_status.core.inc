<?php
/**
 * @file
 * All Drupal 7 modules that are now part of Backdrop core.
 */

/**
 * Modules in core in Backdrop CMS.
 *
 * @see upgrades_status_moved_into_core()
 */
function _backdrop_upgrade_status_backdrop_core(&$projects, $project) {

  // Language used below...
  // - "Moved into core" means the whole module was plunked in.
  // - "Integrated into" means the code/feature was moved into a core module.
  // - "Replaced by" means something else can/should be used.
  // -


  // Specifying a case for the module in this switch statement will mark it as
  // included in core on the status report.
  $core = TRUE;

  switch ($project) {

    case 'admin_menu':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by the core Admin Bar module.');
      break;

    case 'admin_views':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by core views as of 1.0. The Comments listing is not yet a view.');
      break;

    case 'automenu':
    case 'auto_menu_settings':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core Node module in v 1.15. Per content-type defaults not included.');
      break;

    case 'bean':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by page-less nodes placed as blocks as of v1.11');
      break;

    case 'block_class':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core Block module since v1.0');
      break;

    case 'blockify':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by core blocks as of v1.7');
      break;

    case 'boolean_formatter':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core List module in v1.15');
      break;

    case 'boxes':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by core Configuration Management system since v1.0');
      break;

    case 'bt_cbc':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by core blocks: added Hero block in v1.5, and Contact block in v1.12');
      break;

    case 'caption_filter':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into the core CKEditor module in v1.2');
      break;

    case 'ckeditor':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by core CKEditory module as of v1.2. Reconfiguration will be necessary.');
      break;

    case 'ckeditor_link':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core CKEditor module in v1.7');
      break;

    case 'ckeditor_media':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially integrated into the core CKEditor module in 1.11');
      break;

    case 'ckeditor_upload':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into CKEditor module in v1.0 (images) v1.10 (files)');
      break;

    case 'comment_closer':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into Comment module as of v1.10');
      break;

    case 'contact_block':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into Contact module as of v1.12');
      break;

    case 'context':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by the context-aware Layout module since v1.0');

    case 'ctools':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Mostly replaced by core APIs including: modal dialogs, dropbuttons, cofiguration exportables and page handlers (from <a href="@url">Page Manager</a>). Excludes plugins and Form Wizard.', array(
        '@url' => 'https://www.drupal.org/project/page_manager',
      ));
      break;

    case 'date':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Module partially moved into core in v1.2. <a href="@repeat">Date Repeat</a> remains in contrib', array(
        '@repeat' => 'https://github.com/backdrop-contrib/date_repeat',
      ));
      break;

    case 'date_popup_authored':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into Date module in v1.2');
      break;

    case 'defaultconfig':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by core Configuration Management system since v1.0');
      break;

    case 'diff':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Diff engine included as part of Configuration Management system since v1.0');
      break;

    case 'direct_upload':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by better Rich-Text image/file upload options added in v1.7');
      break;

    case 'display_suite':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by Layout module since v1.0. Layouts for non-page displays not yet supported.');
      break;

    case 'elements':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core System module since v1.0');
      break;

    case 'email':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Module moved into core in v1.2. E-mail address contact forms are not supported by core.');
      break;

    case 'email_registration':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core User module since v1.0');
      break;

    case 'entity':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core Entity module since v1.0');
      break;

    case 'entity_view_mode':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core Field UI module in v1.6');
      break;

    case 'extra_columns':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by default views as of 1.0. The Comments listing is not yet a view.');
      break;

    case 'features':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by core Configuration Management system since v1.0. Feaure-set bundles and overridden status not yet supported.');
      break;

    case 'fieldable_panels_panes':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by page-less nodes placed as blocks as of v1.11');
      break;

    case 'fieldblock':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into Field module as of v1.3.');
      break;

    case 'field_formatter_settings':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into Field module as of v1.3.');
      break;

    case 'file_entity':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into File module since v1.0 and v1.6');
      break;

    case 'float_filter':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into the core CKEditor module in v1.2');
      break;

    case 'html5_tools':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core System module since v1.0');
      break;

    case 'insert':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into the core CKEditor module since v1.0');
      break;

    case 'instantfilter':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by search box on the modules page since v1.0');
      break;

    case 'image_mobile_camera':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into the core File module as of v1.4');
      break;

    case 'imce_wysiwyg':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by the core CKEditor module in v1.2');
      break;

    case 'link':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Module moved into core as of v1.2');
      break;

    case 'linkit':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially in core.');
      break;

    case 'live_preview':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by new content preview in core Node module as of v1.11');
      break;

    case 'media':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by core File, CKEditor, and System modules since v1.0');
      break;

    case 'menu_block':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially integrated into System module Menu blocks as of v1.5. Settings other than depth not supported yet.');
      break;

    case 'menu_translation':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core Menu module in v1.10');
      break;

    case 'module_filter':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially integrated into System module since v1.0. A search box is included on the modules page since v1.0');
      break;

    case 'navbar':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by the core Admin Bar module since v1.0');
      break;

    case 'options_element':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core Field module in v1.12');
      break;

    case 'page_preview':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by new content preview in core Node module as of v1.11');
      break;

    case 'panels':
      $projects[$project]['in_core_since'] = '8.5';
      $projects[$project]['in_core_note'] = t('Partially replaced by Layout module since v1.0. Panel nodes are not provided in core.');
      break;

    case 'panels_everywhere':
      $projects[$project]['in_core_since'] = '8.5';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by Layout module since v1.0. Panel nodes are not provided in core.');
      break;

    case 'pathauto':
      $projects[$project]['in_core_since'] = '8.5';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core Path module in v1.11');
      break;

    case 'placeholder':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core System module since v1.0');
      break;

    case 'project_browser':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by core Installer module in v1.4');
      break;

    case 'rabbit_hole':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core Node module (as page-less nodes) in v1.11');
      break;

    case 'redirect':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Module moved into core in v1.4');
      break;

    case 'role_assign':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into User module in v1.6');
      break;

    case 'scheduler':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core Node module in v1.10');
      break;

    case 'smartmenus':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by core System drop-down menus in v1.5 + v1.6');
      break;

    case 'strongarm':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by core Configuration Management system in v1.0');
      break;

    case 'superfish':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by core System drop-down menus in v1.5 + v1.6');
      break;

    case 'token':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Module moved into core in v1.2');
      break;

    case 'transliteration':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core APIs. No direct support for transliterating path aliases or file names.');
      break;

    case 'uuid':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'variable':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Replaced by the core Configuration Management system since v1.0');
      break;

    case 'vertical_tabs_responsive':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core themes in v1.2');
      break;

    case 'views':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'views_bulk_operations':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('The core Views module provides bulk operations on simple actions. No support for batch operations or configurable actions.');
      break;

    case 'views_responsive_grid':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'wysiwyg':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Rich-Text Editor is closely integrated with text formats as of v1.2. Reconfiguration will be necessary.');
      break;

      // Also correct information about D7 modules.
    case 'cck':
      $projects[$project]['in_core_note'] = '';
      $projects[$project]['in_core_complete'] = TRUE;
      break;


    default:
      // Any other module is not included in core.
      $core = FALSE;
  }

  return $core;
}
