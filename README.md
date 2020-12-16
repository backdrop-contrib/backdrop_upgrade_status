Backdrop Upgrade Status
=======================

Backdrop Upgrade Status module was designed to provide an easy way tell when
your website is ready to be upgraded to BAckdrop CMS. The module will compile a
list of your projects along with a status, which can be one of the following:

* Available: A stable release of this project is available.
* In development: A development release of this project is available, which can
be installed for testing purposes.
* Not ported yet: There are no releases available for this project.

Clicking on any of the modules' boxes will expand the area and show you a link
to download the new version of the project, as well as read its release notes.

After the scan
--------------

If one or more of your installed modules are not yet ported to Backdrop, you
should:

1. Search the modules' issue queue for any previous requests for a Backdrop
port.
2. Create an issue in the [Backdrop Contrib issue queue](https://github.com/backdrop-ops/contrib/issues)
to apply as a project contributor. Install the [Coder upgrade module](https://github.com/backdrop-contrib/coder_upgrade),
use its code rules for migrating a module to Backdrop to begin.
3. Or send a port request in the same issue queue.

For a full description visit [the project page](https://www.drupal.org/project/backdrop_upgrade_status).

For bug reports, feature suggestions and latest developments, see the
[issue queue](https://www.drupal.org/project/issues/backdrop_upgrade_status).

Installation
------------

Download the module zip file, unzip the file, and place in your modules folder.
Then go to Administer >> Modules and enable the module.

Usage
-----

Go to Administer >> Reports >> Available updates >> Backdrop upgrade status,
and check the status of your installed modules.

License
-------

This project is GPL v2 software.

Current Maintainers
-------------------

* [Herb v/d Dool](https://www.drupal.org/u/nubeli)
* [laryn](https://www.drupal.org/u/laryn)
* [jenlampton](https://www.drupal.org/u/jenlampton)

Credit
------

Module based on Drupal's Upgrade Status module.
Drupal maintainers of Upgrade Status:

* Daniel F. Kudwien (sun) - dev@unleashedmind.com

This project had been sponsored on Drupal by:

* [UNLEASHED MIND](http://www.unleashedmind.com): Specialized in consulting and
planning of Drupal powered sites, UNLEASHED MIND offers installation, development, theming, customization, and hosting to get you started. Visit them for more information.

Ported by [docwilmot](https://github.com/docwilmot).
