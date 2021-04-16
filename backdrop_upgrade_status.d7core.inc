<?php
/**
 * @file
 * All Drupal modules that are now part of Drupal 7 core.
 */

/**
 * Modules in core since Drupal 7.
 *
 * @see upgrades_status_moved_into_core()
 */
function _backdrop_upgrade_status_d7_core(&$projects, $project) {

  // Specifying a case for the module in this switch statement will mark it as
  // included in core on the status report.
  $core = TRUE;
  switch ($project) {

    case 'ahah_helper':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Allows Backdropmodules to implement AHAH/AJAX functionality without touching JavaScript. Entirely covered by the AJAX framework in Backdrop, which is based on <a href="@ctools-url">CTools</a>.', array(
        '@ctools-url' => 'http://drupal.org/project/ctools',
      ));
      break;

    case 'autoload':
      $projects[$project]['in_core_since'] = '1.x';
      $projects[$project]['in_core_note'] = t('Allows Backdrop modules to lazy-load any class that has not been loaded yet. A major performance improvement in Backdrop, and the only part of the <a href="@issue-url">Registry</a> that was introduced earlier, but later removed from Backdrop.', array(
        '@issue-url' => 'http://drupal.org/node/221964',
      ));
      break;

    case 'auto_menutitle':
    case 'automaticmenu':
    case 'automenu':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('When creating new content, Backdrop automatically takes over the title of the content as menu link title, while still allowing you to alter it.');
      break;

    case 'admin_hover':
    case 'block_edit':
    case 'contextual':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('The new <a href="@issue-url">Contextual links</a> module in Backdrop allows you to manage page elements from the page you are looking at, i.e., you have direct access to "Edit" and "Delete" pages for content, or "Configure" pages for blocks, "List links" for menu blocks, etc. Every module can integrate with Contextual module.', array(
        '@issue-url' => 'http://drupal.org/node/473268',
      ));
      break;

    case 'adminrole':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('The <a href="@issue-url">administration role</a> is based on regular Backdrop user roles and every user role can be configured to be the administration role. All new permissions (and only new) are automatically granted to the configured administration role. Permissions can still be removed from the role.', array(
        '@issue-url' => 'http://drupal.org/node/480660',
      ));
      break;

    case 'advuser':
      $projects[$project]['in_core_since'] = '8.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'block_node_visibility':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'blocks404':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'button_style':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'canonical_url':
    case 'shortlink':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'cck':
    case 'content':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('You still need <a href="@cck-url">CCK</a> for the Nodereference and Userreference field types, and to upgrade your fields to Backdrop. There is an overall CCK to field <a href="@upgrade-issue-url">upgrade path discussion</a>. Contributed modules may use <a href="@field-convert-url">Field converter</a> as dependency to properly upgrade their non-field data to fields. Custom programmed field type modules, which may be obsolete now, might be easier to upgrade using <a href="@migrate-url">Migrate</a> module.', array(
        '@upgrade-issue-url' => 'http://drupal.org/node/366364',
        '@cck-url' => 'http://drupal.org/project/cck',
        '@field-convert-url' => 'http://drupal.org/project/field_convert',
        '@migrate-url' => 'http://drupal.org/project/migrate',
      ));
      break;

    case 'checkbox_validate':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'comment_display':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'config_perms':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'content_dependency':
      $projects[$project]['in_core_since'] = '8.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Use the Relationship feature in Views.');
      break;

    case 'content_taxonomy':
    case 'field_taxonomy':
    case 'term_fields':
    case 'taxidermy':
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_since'] = '7.x';
      break;

    // @todo Allow to target a module (not project).
    case 'date_timezone':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'dbtng':
    case 'transaction':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Backdrop implements an entirely new <a href="@issue-url">database layer</a>, which allows Backdrop to work with any database. Modules are able to alter most database queries and there is support for transactions.', array(
        '@issue-url' => 'http://drupal.org/node/225450',
      ));
      break;

    case 'dragndrop_upload':
      $projects[$project]['in_core_since'] = 'TBD';
      $projects[$project]['in_core_note'] = t('In development for addition to Backdrop core.  See <a href="@issue-url">File Field design update</a> for details. In the meantime, <a href="@contrib1">Plupload integration</a> and/or <a href="@contrib2">PlUPload File Widget</a> can be used.', array(
        '@issue-url' => 'https://www.drupal.org/node/2113931',
        '@contrib1' => 'https://www.drupal.org/project/plupload',
        '@contrib2' => 'https://www.drupal.org/project/plupload_widget',
      ));
      break;

    case 'edit_term':
    case 'taxonomy_intro':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'elements':
    case 'element_themehook':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('In Backdrop, almost all content is generated as "renderable array", which allows to consider any element on a page as atomic, alterable, and themeable element that can be still be altered until it is rendered into a string.');
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'hook_file':
    case 'storage_api':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Backdrop natively uses PHP 5 stream wrappers, which allow to store and access files in almost any location, even remotely. There is also a clean separation between the public and private filesystem, and both can be used at the same time.');
      break;

    case 'filefield':
      // @todo Allow to target a module (not project).
    case 'upload':
    case 'upload_element':
    case 'upload_preview':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('<a href="@issue-url">Upload</a> module has been replaced with File field.', array(
        '@issue-url' => 'http://drupal.org/node/563000',
      ));
      break;

    case 'filter_default':
    case 'input_format_permissions':
    case 'user_default_filter':
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'ife':
      $projects[$project]['in_core_since'] = '8.x';
      $projects[$project]['in_core_note'] = t('See <a href="@url">the documentation</a>.', array(
        '@url' => 'https://www.drupal.org/docs/8/core/modules/inline-form-errors/inline-form-errors-module-overview',
      ));
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'image':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('The most common use-case of <a href="@image-url">Image</a> module, an image field type, is contained in Backdrop core. The Image project is required to <a href="@upgrade-issue-url">upgrade</a> existing data, and its main image module has been renamed to image_node module, since Backdrop core took over the namespace. Image Node module is required for use-cases like restricting access, direct image access through a URL, attached data like comments, votes, or geo coordinates.', array(
        '@image-url' => 'http://drupal.org/project/image',
        '@upgrade-issue-url' => 'http://drupal.org/node/513096',
      ));
      break;

    case 'imageapi':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('The <a href="@imageapi-url">ImageAPI</a> module for Backdrop only provides the ImageMagick toolkit and an unsharpen mask action. Everything else has been moved into Backdrop core.', array(
        '@imageapi-url' => 'http://drupal.org/project/imageapi',
      ));
      break;

    case 'imagecache':
    case 'imagefield':
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_since'] = '7.x';
      break;

    case 'jq':
    case 'jqp':
    case 'plugins':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('Backdrop allows modules to register custom libraries, consisting of JavaScript and CSS files, which can then be loaded at once. External libraries, i.e., code that is not shipped with a module, is not supported by Backdrop core and requires the <a href="@libraries-url">Libraries API</a> module.', array(
        '@libraries-url' => 'http://drupal.org/project/libraries',
      ));
      break;

    case 'jquery_cookie':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'jquery_form_update':
    case 'jsalter':
    case 'wysiwygcck':
    case 'tinymce_ahah':
    case 'tinymce_dragdrop':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'jquery_ui':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('Advanced jQuery UI features (like Theme Roller support) are not in Drupal core.');
      break;

    case 'jquery_update':
    case 'jqmulti':
      $projects[$project]['in_core_since'] = '8.x';
      $projects[$project]['in_core_note'] = t('No longer needed in Backdrop. The plan is to continue increasing the bundled version of jQuery, with backwards compatibility, in minor releases.');
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'login_security':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('Backdrop core provides no UI. If required, the internal variables may be configured using the <a href="@flood-control-url">Flood control</a> module.', array(
        '@flood-control-url' => 'http://drupal.org/project/flood_control',
      ));
      break;

    case 'media':
      $projects[$project]['in_core_since'] = '8.4';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'menuless_nodetype':
    case 'ctm':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'multiupload_filefield_widget':
      $projects[$project]['in_core_since'] = '8.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'nodepreview_by_type':
    case 'preview':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'permissions_api':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'phpass':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('All stored user passwords will be additionally salted');
      break;

    case 'plugin_manager':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'poormanscron':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'protect_critical_users':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'drupal_queue':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'rdf':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'seven':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'simplecdn':
    case 'abssrc':
    case 'parallel':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_note'] = t('The new File API and handling of JavaScript  and CSS in Backdrop allows modules to alter the URLs of all files. Everyone is encouraged to switch to the joined community effort, the <a href="@cdn-url">CDN</a> project.', array(
        '@cdn-url' => 'http://drupal.org/project/cdn',
      ));
      break;

    case 'simpletest':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'tar':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'taxonomy_delegate':
    case 'vocabperms':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'url_alter':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'user_cancellation':
    case 'user_delete':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'vertical_tabs':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'view_unpublished':
      $projects[$project]['in_core_since'] = '7.x';
      $projects[$project]['in_core_complete'] = TRUE;
      break;

    case 'workflow':
    case 'revisioning':
    case 'workbench_files':
    case 'workbench_media':
    case 'workbench_moderation':
      $projects[$project]['in_core_since'] = '8.5';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('See the <a href="@url">Content Moderation documentation</a>.', array(
        '@url' => 'https://www.drupal.org/docs/8/core/modules/content-moderation/overview',
      ));
      break;

    case 'workspace':
      $projects[$project]['in_core_since'] = '8.6';
      $projects[$project]['in_core_note'] = t('Renamed to <a href="@workspaces">Workspaces</a>.', array(
        '@workspaces' => 'https://www.drupal.org/node/2968491',
      ));
      break;

    case 'wysiwyg_filter':
      $projects[$project]['in_core_since'] = '8.0';
      $projects[$project]['in_core_complete'] = TRUE;
      $projects[$project]['in_core_note'] = t('See the <a href="@url">Text Editor documentation</a>.', array(
        '@url' => 'https://www.drupal.org/docs/8/core/modules/editor/overview',
      ));
      break;

    default:
      // Any other module is not included in core.
      $core = FALSE;
  }

  return $core;
}
