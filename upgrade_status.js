// $Id$

/**
 * Attach collapsible behaviour.
 */
Drupal.upgradeStatusAttach = function() {
  $('table.upgrade-status .project:not(.processed)').each(function() {
    $(this).addClass('upgrade-status-processed')
      .click(function() {
        $('.details-wrapper', this).slideToggle('fast');
      })
      .children('.details-wrapper').hide();
  });
};

// Global killswitch.
if (Drupal.jsEnabled) {
  $(document).ready(Drupal.upgradeStatusAttach);
}

