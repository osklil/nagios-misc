check_tsm
=========

Introduction
------------

check_tsm is a Nagios plugin that performs various checks against IBM Tivoli
Storage Manager (TSM) servers.  It can check number of online drives and
paths, database and log space, as well as number of free, reclaimable or
offline volumes, and if there are outstanding requests.  check_tsm uses
dsmadmc to do its work, but it automatically sets up dsmadmc's environment
and configuration files.  All checks are done in the same session to improve
performance.

check_tsm is written in Perl.

Author and Feedback
-------------------

check_tsm is written by Oskar Liljeblad <oskar@osk.mine.nu>.

This software is a work in progress and there are probably many ways it can
still be improved. If you'd like to contribute patches, ideas, or bug
reports, please send me an email!

License
-------

check_tsm is copyright (C) 2011 Oskar Liljeblad.

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

The latest code for check_snmp_if can be downloaded from GitHub:

 * <https://github.com/osklil/nagios-misc/zipball/master>

Requirements
------------

 * Perl 5.10.0 (or later).
   Debian: perl

 * Net::SNMP Perl module.
   Debian: libnet-snmp-perl

 * Nagios::Plugin Perl module.
   Debian: libnagios-plugin-perl

 * IBM Tivoli Storage Manager Backup-Archive Client 6.2 (or later).
   Source: <ftp://ftp.software.ibm.com/storage/tivoli-storage-management/maintenance/client/v6r2/Linux>

Usage
-----

Run `check_tsm --help' for usage information.

Future
------

Please see the TODO file.

References
----------

 * IBM Tivoli Storage Manager, Version 6.2: Administering the server from the command-line interface
   <http://publib.boulder.ibm.com/infocenter/tsminfo/v6r2/topic/com.ibm.itsm.srv.ref.doc/t_cmdline.html>

 * Nagios plug-in development guidelines.
   <http://nagiosplug.sourceforge.net/developer-guidelines.html>
