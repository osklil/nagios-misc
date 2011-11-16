check_snmp_if
=============

Introduction
------------

check_snmp_if is a Nagios plugin that checks interfaces via SNMP (IF-MIB). 
Unlike many other scripts, it is designed to check all interfaces network
devices that support IF-MIB (such as switches, routers, firewalls).
Exclusions can be made based on a variety of fields, including interface
name, alias and type.

To name a few things, check_snmp_if checks interface status, promiscuous
mode, input and output bandwidth utilization, and broadcast and error packet
levels.

In order to reduce execution time, SNMP requests can be performed in
parallel, so that distant devices with hundreds of interfaces can be checked
in just a few seconds.

SNMP version 1 is not supported because requests are aborted if a
non-supported object is encountered.  There are other reasons for not using
SNMP version 1 - one is that it does not support 64-bit values.  In the
author's experience however, most devices that support SNMP version 1 also
support version 2 - usually without modifications.

License
-------

check_snmp_if is copyright (C) 2011 Oskar Liljeblad.

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

More
----

The remainder of this document is yet to be written.
