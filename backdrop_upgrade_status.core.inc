<?php
/**
 * @file
 * All Drupal 7 modules that are now part of Backdrop core.
 */

/**
 * Modules in core in Backdrop CMS.
 *
 * The following was copied from backdrop_upgrade_status_moved_into_core().
 *
 * @param $data
 *   Array of projects from backdrop_upgrade_status_calculate_project_data(). This
 *   parameter is passed by reference, and metadata for the project can added
 *   to the $data[$project] array for use later. Several additional keys are
 *   supported:
 *   - in_core_since: The major version where the project was added into core.
 *   - in_core_note: Note to display to the user. This should be succinct and
 *     describe:
 *     - If the project was "Moved", "Integrated", or "Replaced by" another.
 *     - What core system, module, or API replaces the project.
 *     - What minor version the project was added into.
 *   - in_core_complete: Boolean flag indicating whether the complete
 *     functionality of the project is in core. Set this to FALSE when the core
 *     replacement does not include the full functionality of the project.
 *   - in_core_warning:
 *     - If the 'in_core_complete' flag is false, what functionality of the
 *       project is not included in core.
 *     - Other things developers will need to know about updating this project.
 *
 * @param $project
 *   Project name to check.
 *
 * @return
 *   TRUE if project has been moved into core.
 *
 * @see backdrop_upgrade_status_moved_into_core()
 */
function _backdrop_upgrade_status_backdrop_core(&$data, $project) {


  // Specifying a case for the module in this switch statement will mark it as
  // included in core on the status report.
  $core = TRUE;

  switch ($project) {

    case 'admin_menu':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by the core Admin Bar module.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');

      break;

    case 'admin_views':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by core views in 1.0.');
      $data[$project]['in_core_warning'] = t('The Comments listing is not yet a view.');
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'automenu':
    case 'auto_menu_settings':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core Node module in v 1.15. Per content-type defaults not included.');
      break;

    case 'bean':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by page-less nodes placed as blocks in v1.11.');
      break;

    case 'block_class':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core Block module in v1.0.');
      $data[$project]['in_core_complete'] = TRUE;
      break;

    case 'blockify':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by core blocks in v1.7.');
      $data[$project]['in_core_complete'] = TRUE;
      break;

    case 'boolean_formatter':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core List module in v1.15.');
      $data[$project]['in_core_complete'] = TRUE;
      break;

    case 'boxes':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by core Configuration Management system in v1.0.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $data[$project]['in_core_upgrade'] = t('Before upgrade: Boxes will need to be converted back into Blocks.');
      break;

    case 'bt_cbc':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially replaced by core blocks: added Hero block in v1.5, and Contact block in v1.12.');
      break;

    case 'caption_filter':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into the core CKEditor module in v1.2.');
      $data[$project]['in_core_complete'] = TRUE;
      break;

    case 'ckeditor':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by core CKEditory module in v1.2.');
      $data[$project]['in_core_warning'] = t('Reconfiguration will be necessary.');
      break;

    case 'ckeditor_link':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core CKEditor module in v1.7.');
      break;

    case 'ckeditor_media':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially integrated into the core CKEditor module in 1.11.');
      // @todo
      break;

    case 'ckeditor_upload':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into CKEditor module in for images in v1.2, and for files v1.10.');
      break;

    case 'comment_closer':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into Comment module in v1.10.');
      $data[$project]['in_core_complete'] = TRUE;
      break;

    case 'contact_block':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into Contact module in v1.12.');
      $data[$project]['in_core_complete'] = TRUE;
      break;

    case 'context':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially replaced by the context-aware Layout module in v1.0.');
      $data[$project]['in_core_warning'] = t('A User Interface for defining custom contexts is not yet available.');
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $data[$project]['in_core_upgrade'] = t('After upgrade: Contexts will need to be re-created as Layouts.');
      break;

    case 'ctools':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially replaced by core APIs including: modal dialogs, dropbuttons, cofiguration exportables, and page handlers (from <a href="@url">Page Manager</a>).', array(
        '@url' => 'https://www.drupal.org/project/page_manager',
      ));
      $data[$project]['in_core_warning'] = t('Plugins (replaced by info hooks) and Form Wizard are not supported.');
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'date':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially moved into core in v1.2.');
      $data[$project]['in_core_warning'] = t('<a href="@repeat">Date Repeat</a> remains in contrib.', array(
        '@repeat' => 'https://github.com/backdrop-contrib/date_repeat',
      ));
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'date_popup_authored':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into Date module in v1.2.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'defaultconfig':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by core Configuration Management system in v1.0.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'diff':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Parially Integrated into Configuration Management system in v1.0.');
      $data[$project]['in_core_warning'] = t('Integration with the node revision system <a href="@diff">remains in contrib</a>. Only the diff engine was included.', array(
        '@diff' => url('https://github.com/backdrop-contrib/diff'),
      ));
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'direct_upload':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by Rich-Text image/file upload options added in v1.7.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'ds':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially replaced by Layout module in v1.0.');
      $data[$project]['in_core_warning'] = t('Layouts for view modes are not yet supported.');
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $data[$project]['in_core_upgrade'] = t('After upgrade: New Layouts will need to be configured.');
      break;

    case 'elements':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core System module in v1.0.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'email':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Moved into core in v1.2. E-mail address contact forms are not supported by core.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'email_registration':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core User module in v1.0.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'entity':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core Entity module in v1.0.');
      $data[$project]['in_core_warning'] = t('The Entity Metadata Wrapper is not included but <a href="@entity_plus">is available in contrib</a>.', array(
        '@diff' => url('https://github.com/backdrop-contrib/entity_plus'),
      ));
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'entity_view_mode':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core Field UI module in v1.6.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'extra_columns':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially replaced by default views in 1.0.');
      $data[$project]['in_core_warning'] = t('The Comments listing is not yet a view. See the <a href="@views">relevant core issue</a>.', array(
        '@views' => 'https://github.com/backdrop/backdrop-issues/issues/151',
      ));
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'features':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially replaced by core Configuration Management system in v1.0.');
      $data[$project]['in_core_warning'] = t('Recipes (formerly named "Features") are not yet supported. See the <a href="@recipes">relevant core issue</a>.', array(
        '@recipes' => 'https://github.com/backdrop/backdrop-issues/issues/3763',
      ));
      $data[$project]['in_core_upgrade_status'] = t('Disable and Uninstall');
      $data[$project]['in_core_upgrade'] = t('Before upgrade: Store all features back in the database.');
      break;

    case 'fieldable_panels_panes':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by page-less nodes placed as blocks in v1.11. This setting was provided by Rabbit Hole module in Drupal 7.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $data[$project]['in_core_upgrade'] = t('Before upgrade: convert all Fieldable panes into nodes.');
      break;

    case 'fieldblock':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into Field module in v1.3.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'field_formatter_settings':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into Field module in v1.3.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'file_entity':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into File module in v1.0 and v1.6.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'float_filter':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into the core CKEditor module in v1.2.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'html5_tools':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core System module in v1.0.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'insert':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into the core CKEditor module in v1.2.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'instantfilter':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by search box on the modules page in v1.0.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'image_mobile_camera':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into the core File module in v1.4.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'imce_wysiwyg':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by the core CKEditor module for images in v1.2, and for files in v1.10.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'link':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Moved into core in v1.2.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'linkit':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially in core.');
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'live_preview':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by new content preview in core Node module in v1.11.');
      $data[$project]['in_core_complete'] = TRUE;
      break;

    case 'media':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially replaced by core System module in v1.0, CKEditor module in v.1.2, and File module in v1.10.');
      $data[$project]['in_core_warning'] = t('Media entites are not included (File entites should be sufficient).');
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'menu_block':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially integrated into System module Menu blocks in v1.5.');
      $data[$project]['in_core_warning'] = t('Settings other than depth not supported yet.');
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'menu_translation':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_note'] = t('Integrated into core Menu module in v1.10.');
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'module_filter':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially integrated into System module in v1.0.');
      $data[$project]['in_core_warning'] = t('Vertical tabs on the modules page are not included.');
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'navbar':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by the core Admin Bar module in v1.0.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'options_element':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core Field module in v1.12.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'page_preview':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by new content preview in core Node module in v1.11.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'panels':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Partially replaced by Layout module in v1.0.');
      $data[$project]['in_core_warning'] = t('Layouts saved as content are not provided in core (Panel Nodes).');
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $data[$project]['in_core_upgrade'] = t('After upgrade: Layouts will need to be configured to replace Panels Pages.');
      break;

    case 'panels_everywhere':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by Layout module in v1.0. Panel nodes are not provided in core.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $data[$project]['in_core_upgrade'] = t('After upgrade: Layouts will need to be configured to replace Panels Pages.');
      break;

    case 'pathauto':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core Path module in v1.11.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'placeholder':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core System module in v1.0.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'project_browser':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by core Installer module in v1.4.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'rabbit_hole':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core Node module (as page-less nodes) in v1.11.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'redirect':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Moved into core in v1.4.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'role_assign':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into User module in v1.6.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'scheduler':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core Node module in v1.10.');
      $data[$project]['in_core_warning'] = t('Scheduled un-publishing is still <a href="@scheduler">available in contrib</a>.', array(
        '@scheduler' => 'https://github.com/backdrop-contrib/scheduler',
      ));
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'smartmenus':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by core System drop-down menus in v1.5 + v1.6.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'strongarm':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by core Configuration Management system in v1.0.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'superfish':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by core System drop-down menus in v1.5 + v1.6.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'token':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into System module in v1.2.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'transliteration':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core APIs.');
      $data[$project]['in_core_warning'] = t('No direct support for transliterating path aliases or file names.');
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'uuid':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'variable':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Replaced by the core Configuration Management system in v1.0.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'vertical_tabs_responsive':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into core themes in v1.2.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'views':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Moved into core in v1.0.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      $data[$project]['in_core_upgrade'] = t('Before upgrade: Store all views in the database (not in exports).');
      break;

    case 'views_bulk_operations':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('The core Views module provides bulk operations on simple actions.');
      $data[$project]['in_core_warning'] = t('No support for batch operations or configurable actions. See the relevant <a href="@vbo">core issue</a>.', array(
        '@vbo' => 'https://github.com/backdrop/backdrop-issues/issues/3797',
       ));
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'views_responsive_grid':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Integrated into the Views module (Grid style) in v1.12.');
      $data[$project]['in_core_complete'] = TRUE;
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'wysiwyg':
      $data[$project]['in_core_since'] = '1.x';
      $data[$project]['in_core_note'] = t('Rich-Text Editor closely integrated with text formats in v1.2.');
      $data[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $data[$project]['in_core_upgrade'] = t('After upgrade: Editor reconfiguration will be necessary.');
      break;

      // Also correct information about D7 modules.
    case 'cck':
      $data[$project]['in_core_note'] = '';
      $data[$project]['in_core_complete'] = TRUE;
      break;


    default:
      // Any other module is not included in core.
      $core = FALSE;
  }

  return $core;
}
