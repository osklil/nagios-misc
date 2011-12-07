check_snmp_hr_lm
================

Introduction
------------

check_snmp_hr_lm is a set of Nagios plugin for Host Resources and LAN
Manager SNMP checks:

 * check_snmp_lm_services
   Check running services using LanMgr-Mib-II-MIB SNMP requests, as
   supported on most Microsoft Windows systems.

 * check_snmp_hr_processes
   Check running processes using HOST-RESOURCES-V2-MIB SNMP requests.

The check_snmp_hr_lm plugins are written in Perl.

Author and Feedback
-------------------

The check_snmp_hr_lm plugins are written by Oskar Liljeblad <oskar@osk.mine.nu>.

This software is a work in progress and there are probably many ways it can
still be improved. If you'd like to contribute patches, ideas, or bug
reports, please send me an email!

License
-------

check_snmp_hr_lm is copyright (C) 2011 Oskar Liljeblad.

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

The latest code for check_snmp_hr_lm can be downloaded from GitHub:

 * <https://github.com/osklil/nagios-misc/zipball/master>

The Git repository is hosted on GitHub:

 * <https://github.com/osklil/nagios-misc>

Requirements
------------

 * Perl 5.10.0 (or later).
   Debian: perl

 * Net::SNMP Perl module.
   Debian: libnet-snmp-perl

 * Nagios::Plugin Perl module.
   Debian: libnagios-plugin-perl

Usage
-----

Run each plugin with the --help option for usage information.

Future
------

Please see the TODO file.

References
----------

 * RFC 2790 - Host Resources MIB.
   <http://tools.ietf.org/html/rfc2790>

 * Nagios plug-in development guidelines.
   <http://nagiosplug.sourceforge.net/developer-guidelines.html>

 * LanMgr-Mib-II from Appendix C - MIB Object Types for Windows NT, Windows NT Server Resource Kit.
   <http://www.microsoft.com/resources/documentation/windowsnt/4/server/reskit/en-us/net/sur_mib.mspx>.
   The MIB file is available in %systemroot%\system32\LMMIB2.MIB on Windows systems where the SNMP
   Server is installed.
