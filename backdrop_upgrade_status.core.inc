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
 * @param $projects
 *   Array of projects from backdrop_upgrade_status_calculate_project_data(). This
 *   parameter is passed by reference, and metadata for the project can added
 *   to the $projects[$project] array for use later. Several additional keys are
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
function _backdrop_upgrade_status_backdrop_core(&$projects, $project) {


  // Specifying a case for the module in this switch statement will mark it as
  // included in core on the status report.
  $core = TRUE;

  switch ($project) {

    case 'admin_menu':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by the core Admin Bar module.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');

      break;

    case 'admin_views':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by core views in 1.0.');
      $projects[$project]['in_core_warning'] = t('The Comments listing is not yet a view.');
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'automenu':
    case 'auto_menu_settings':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core Node module in v 1.15. Per content-type defaults not included.');
      break;

    case 'bean':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by page-less nodes placed as blocks in v1.11.');
      break;

    case 'block_class':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core Block module in v1.0.');
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'blockify':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by core blocks in v1.7.');
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'boolean_formatter':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core List module in v1.15.');
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'boxes':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by core Configuration Management system in v1.0.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $projects[$project]['in_core_upgrade'] = t('Before upgrade: Boxes will need to be converted back into Blocks.');
      break;

    case 'bt_cbc':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by core blocks: added Hero block in v1.5, and Contact block in v1.12.');
      break;

    case 'caption_filter':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into the core CKEditor module in v1.2.');
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'ckeditor':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by core CKEditory module in v1.2.');
      $projects[$project]['in_core_warning'] = t('Reconfiguration will be necessary.');
      break;

    case 'ckeditor_link':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core CKEditor module in v1.7.');
      break;

    case 'ckeditor_media':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially integrated into the core CKEditor module in 1.11.');
      // @todo
      break;

    case 'ckeditor_upload':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into CKEditor module in for images in v1.2, and for files v1.10.');
      break;

    case 'comment_closer':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into Comment module in v1.10.');
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'contact_block':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into Contact module in v1.12.');
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'context':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by the context-aware Layout module in v1.0.');
      $projects[$project]['in_core_warning'] = t('A User Interface for defining custom contexts is not yet available.');
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $projects[$project]['in_core_upgrade'] = t('After upgrade: Contexts will need to be re-created as Layouts.');
      break;

    case 'ctools':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by core APIs including: modal dialogs, dropbuttons, cofiguration exportables, and page handlers (from <a href="@url">Page Manager</a>).', array(
        '@url' => 'https://www.drupal.org/project/page_manager',
      ));
      $projects[$project]['in_core_warning'] = t('Plugins (replaced by info hooks) and Form Wizard are not supported.');
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'date':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially moved into core in v1.2.');
      $projects[$project]['in_core_warning'] = t('<a href="@repeat">Date Repeat</a> remains in contrib.', array(
        '@repeat' => 'https://github.com/backdrop-contrib/date_repeat',
      ));
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'date_popup_authored':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into Date module in v1.2.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'defaultconfig':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by core Configuration Management system in v1.0.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'diff':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Parially Integrated into Configuration Management system in v1.0.');
      $projects[$project]['in_core_warning'] = t('Integration with the node revision system <a href="@diff">remains in contrib</a>. Only the diff engine was included.', array(
        '@diff' => url('https://github.com/backdrop-contrib/diff'),
      ));
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'direct_upload':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by Rich-Text image/file upload options added in v1.7.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'ds':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by Layout module in v1.0.');
      $projects[$project]['in_core_warning'] = t('Layouts for view modes are not yet supported.');
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $projects[$project]['in_core_upgrade'] = t('After upgrade: New Layouts will need to be configured.');
      break;

    case 'elements':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core System module in v1.0.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'email':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Moved into core in v1.2. E-mail address contact forms are not supported by core.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'email_registration':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core User module in v1.0.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'entity':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core Entity module in v1.0.');
      $projects[$project]['in_core_warning'] = t('The Entity Metadata Wrapper is not included but <a href="@entity_plus">is available in contrib</a>.', array(
        '@diff' => url('https://github.com/backdrop-contrib/entity_plus'),
      ));
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'entity_view_mode':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core Field UI module in v1.6.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'extra_columns':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by default views in 1.0.');
      $projects[$project]['in_core_warning'] = t('The Comments listing is not yet a view. See the <a href="@views">relevant core issue</a>.', array(
        '@views' => 'https://github.com/backdrop/backdrop-issues/issues/151',
      ));
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'features':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by core Configuration Management system in v1.0.');
      $projects[$project]['in_core_warning'] = t('Recipes (formerly named "Features") are not yet supported. See the <a href="@recipes">relevant core issue</a>.', array(
        '@recipes' => 'https://github.com/backdrop/backdrop-issues/issues/3763',
      ));
      $projects[$project]['in_core_upgrade_status'] = t('Disable and Uninstall');
      $projects[$project]['in_core_upgrade'] = t('Before upgrade: Store all features back in the database.');
      break;

    case 'fieldable_panels_panes':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by page-less nodes placed as blocks in v1.11. This setting was provided by Rabbit Hole module in Drupal 7.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $projects[$project]['in_core_upgrade'] = t('Before upgrade: convert all Fieldable panes into nodes.');
      break;

    case 'fieldblock':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into Field module in v1.3.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'field_formatter_settings':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into Field module in v1.3.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'file_entity':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into File module in v1.0 and v1.6.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'float_filter':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into the core CKEditor module in v1.2.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'html5_tools':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core System module in v1.0.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'insert':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into the core CKEditor module in v1.2.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'instantfilter':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by search box on the modules page in v1.0.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'image_mobile_camera':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into the core File module in v1.4.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'imce_wysiwyg':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by the core CKEditor module for images in v1.2, and for files in v1.10.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'link':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Moved into core in v1.2.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'linkit':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially in core.');
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'live_preview':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by new content preview in core Node module in v1.11.');
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'media':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by core System module in v1.0, CKEditor module in v.1.2, and File module in v1.10.');
      $projects[$project]['in_core_warning'] = t('Media entites are not included (File entites should be sufficient).');
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'menu_block':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially integrated into System module Menu blocks in v1.5.');
      $projects[$project]['in_core_warning'] = t('Settings other than depth not supported yet.');
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'menu_translation':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Integrated into core Menu module in v1.10.');
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'module_filter':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially integrated into System module in v1.0.');
      $projects[$project]['in_core_warning'] = t('Vertical tabs on the modules page are not included.');
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'navbar':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by the core Admin Bar module in v1.0.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'options_element':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core Field module in v1.12.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'page_preview':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by new content preview in core Node module in v1.11.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'panels':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Partially replaced by Layout module in v1.0.');
      $projects[$project]['in_core_warning'] = t('Layouts saved as content are not provided in core (Panel Nodes).');
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $projects[$project]['in_core_upgrade'] = t('After upgrade: Layouts will need to be configured to replace Panels Pages.');
      break;

    case 'panels_everywhere':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by Layout module in v1.0. Panel nodes are not provided in core.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $projects[$project]['in_core_upgrade'] = t('After upgrade: Layouts will need to be configured to replace Panels Pages.');
      break;

    case 'pathauto':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core Path module in v1.11.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'placeholder':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core System module in v1.0.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'project_browser':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by core Installer module in v1.4.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'rabbit_hole':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core Node module (as page-less nodes) in v1.11.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'redirect':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Moved into core in v1.4.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'role_assign':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into User module in v1.6.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'scheduler':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core Node module in v1.10.');
      $projects[$project]['in_core_warning'] = t('Scheduled un-publishing is still <a href="@scheduler">available in contrib</a>.', array(
        '@scheduler' => 'https://github.com/backdrop-contrib/scheduler',
      ));
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'smartmenus':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by core System drop-down menus in v1.5 + v1.6.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'strongarm':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by core Configuration Management system in v1.0.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'superfish':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by core System drop-down menus in v1.5 + v1.6.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'token':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into System module in v1.2.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'transliteration':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core APIs.');
      $projects[$project]['in_core_warning'] = t('No direct support for transliterating path aliases or file names.');
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      break;

    case 'uuid':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'variable':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Replaced by the core Configuration Management system in v1.0.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'vertical_tabs_responsive':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into core themes in v1.2.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'views':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Moved into core in v1.0.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Leave enabled.');
      $projects[$project]['in_core_upgrade'] = t('Before upgrade: Store all views in the database (not in exports).');
      break;

    case 'views_bulk_operations':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('The core Views module provides bulk operations on simple actions.');
      $projects[$project]['in_core_warning'] = t('No support for batch operations or configurable actions. See the relevant <a href="@vbo">core issue</a>.', array(
        '@vbo' => 'https://github.com/backdrop/backdrop-issues/issues/3797',
       ));
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'views_responsive_grid':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Integrated into the Views module (Grid style) in v1.12.');
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      break;

    case 'wysiwyg':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Rich-Text Editor closely integrated with text formats in v1.2.');
      $projects[$project]['in_core_upgrade_status'] = t('Disable and uninstall.');
      $projects[$project]['in_core_upgrade'] = t('After upgrade: Editor reconfiguration will be necessary.');
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
