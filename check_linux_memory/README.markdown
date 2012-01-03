check_linux_memory
==================

Introduction
------------

check_linux_memory is a Nagios plugin that checks amount of free/used memory
and swap in Linux systems. It does not count "buffers" or "cached" memory
as memory in use.

check_linux_memory is written in Perl.

Author and Feedback
-------------------

check_linux_memory is written by Oskar Liljeblad <oskar@osk.mine.nu>.

This software is a work in progress and there are probably many ways it can
still be improved. If you'd like to contribute patches, ideas, or bug
reports, please send me an email!

License
-------

check_linux_memory is copyright (C) 2012 Oskar Liljeblad.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Download
--------

The latest code for check_snmp_cisco can be downloaded from GitHub:

 * <https://github.com/osklil/nagios-misc/zipball/master>

The Git repository is hosted on GitHub:

 * <https://github.com/osklil/nagios-misc>

Requirements
------------

 * Perl 5.10.0 (or later).
   Debian: perl

 * Nagios::Plugin Perl module.
   Debian: libnagios-plugin-perl

Usage
-----

Run check_linux_memory with the --help option for usage information.

Future
------

Please see the TODO file.

References
----------

 * Nagios plug-in development guidelines.
   <http://nagiosplug.sourceforge.net/developer-guidelines.html>
