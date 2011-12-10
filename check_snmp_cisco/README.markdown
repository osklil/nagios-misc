check_snmp_cisco
================

Introduction
------------

check_snmp_cisco is a set of Nagios plugin for checking Cisco network
devices, such as switches, routers, and firewalls. The following
checks are provided:

 * check_snmp_cisco_cpu
   Check CPU utilization using CISCO-PROCESS-MIB.

 * check_snmp_cisco_memory
   Check memory usage using CISCO-MEMORY-POOL-MIB.

The check_snmp_cisco plugins are written in Perl.

Author and Feedback
-------------------

The check_snmp_cisco plugins are written by Oskar Liljeblad <oskar@osk.mine.nu>.

This software is a work in progress and there are probably many ways it can
still be improved. If you'd like to contribute patches, ideas, or bug
reports, please send me an email!

License
-------

check_snmp_cisco is copyright (C) 2011 Oskar Liljeblad.

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

 * Cisco Technote 15215: "How to Collect CPU Utilization on Cisco IOS Devices Using SNMP".
   <http://www.cisco.com/en/US/tech/tk648/tk362/technologies_tech_note09186a0080094a94.shtml>

 * CISCO-PROCESS-MIB: "MIB for CPU and process statistics".
   <ftp://ftp.cisco.com/pub/mibs/v2/CISCO-PROCESS-MIB.my>

 * CISCO-MEMORY-POOL-MIB: "MIB module for monitoring memory pools".
   <ftp://ftp.cisco.com/pub/mibs/v2/CISCO-MEMORY-POOL-MIB.my>

 * Nagios plug-in development guidelines.
   <http://nagiosplug.sourceforge.net/developer-guidelines.html>
