check_by_ssh_master
===================

Introduction
------------

check_by_ssh_master is a Nagios plugin that allows plugins to be executed on
remote hosts over SSH.  It is similar to check_by_ssh but has two
advantages: It uses one or more master connections to improve speed, and it
supports escaping of parameters which may be necessary because SSH executes
commands through the shell.

check_by_ssh_master is written in C (GNU GCC required).

Author and Feedback
-------------------

check_by_ssh_master is written by Oskar Liljeblad <oskar@osk.mine.nu>.

This software is a work in progress and there are probably many ways it can
still be improved. If you'd like to contribute patches, ideas, or bug
reports, please send me an email!

License
-------

check_by_ssh_master is copyright (C) 2011 Oskar Liljeblad.

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

Build requirements:

 * GNU C compiler (gcc), 4.0 or later.
   Debian: gcc

 * make.
   Debian: make

Runtime requirements:

 * GNU C Library (glibc).
   Debian: libc

Usage
-----

Run `check_by_ssh_master --help' for usage information.

Future
------

Please see the TODO file.

References
----------

 * Nagios plug-in development guidelines.
   <http://nagiosplug.sourceforge.net/developer-guidelines.html>

 * Nagios Plugins.
   <http://nagiosplug.sourceforge.net/>
