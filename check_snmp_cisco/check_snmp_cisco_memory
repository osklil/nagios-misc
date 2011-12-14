#!/usr/bin/perl -w
#
# check_snmp_cisco_memory - Check memory usage on Cisco devices
#
# Copyright (C) 2011 Oskar Liljeblad
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Run check_snmp_cisco_memory --help for usage information.
# Written by Oskar Liljeblad <oskar@osk.mine.nu>.
#

require 5.10.0;
use strict;
use Net::SNMP;
use List::Util qw(max);
use Getopt::Long;
use Nagios::Plugin;
use Nagios::Plugin::Threshold;

my $PROGRAM = 'check_snmp_cisco_memory';
my $VERSION = '0.0+git';

my $plugin = Nagios::Plugin->new(shortname => $PROGRAM);
my %opt = (
  'snmp-version' => 1,
  'timeout' => 30,
  'verbose' => 0,
  'warning' => '',
  'critical' => '',
);
Getopt::Long::config('bundling');
Getopt::Long::GetOptions(\%opt,
  'host|H=s', 'timeout|t=i', 'community|C=s', 'snmp-version|V=s',
  'auth-username=s', 'auth-password=s', 'priv-password=s',
  'auth-protocol=s', 'priv-protocol=s', 'port=i', 'snmp-message-size=i',
  'local-address=s', 'local-port=s', 'filter|F=s', 'no-perfdata|n',
  'verbose|v+', 'warning|w=s', 'critical|c=s',
  'list|l', 'help', 'version'
) || exit UNKNOWN;

if (defined $opt{'help'}) {
  print <<"__END__";
Usage: $PROGRAM -H ADDRESS [OPTION]...
Check memory usage on Cisco devices via SNMP (CISCO-MEMORY-POOL-MIB).

General options:
  -H, --host=ADDRESS               address or name of host to check
  -v, --verbose                    explain what is being done
  -F, --filter=FILTER              include/exclude pools by index
  -w, --warning=THRESHOLDS         warning threshold for utilization
  -c, --critical=THRESHOLDS        critical threshold for utilization
  -n, --no-perfdata                do not print any performance data
  -l, --list                       list memory pools and do not check
      --help                       display this help and exit
      --version                    output version information and exit

SNMP options:
  -V, --snmp-version=VERSION       SNMP version (1, 2c, 3), default is 1
  -t, --timeout=SECONDS            max time for individual SNMP requests
      --port=NUMBER                SNMP port (if other than 161)
      --local-address=ADDRESS      local address for outgoing packets
      --local-port=NUMBER          local port for outgoing packets
  -C, --community=NAME             SNMP community
      --auth-username=USER         authentication username (SNMPv3)
      --auth-password=PASSWORD     authentication password (SNMPv3)
      --auth-protocol=md5|sha      authentication protocol (SNMPv3)
      --priv-password=PASSWORD     encryption password (SNMPv3)
      --priv-protocol=des|aes      encryption protocol (SNMPv3)
      --snmp-message-size=BYTES    max size of SNMP request packets

Multiple thresholds can be specified separated by comma. Use this in
combination with --filter/-F to specify different thresholds for
different pools.

Thresholds follows the usual Nagios format:
  Value                  Okay if
  95                     0 <= x <= 95
  95:                    x >= 95
  ~:95                   x <= 95
  50:95                  50 <= x <= 95
  \@50:95                 x < 50 or x > 95

Examples:
  $PROGRAM -H myhost -C mycommunity -w 90 -c 95

Report $PROGRAM bugs to <oskar\@osk.mine.nu>.
__END__
  exit;
}
if (defined $opt{'version'}) {
  print "$PROGRAM $VERSION\n";
  print "Copyright (C) 2011 Oskar Liljeblad\n";
  print "License AGPLv3+: GNU Affero GPL version 3 or later <http://gnu.org/licenses/agpl.html>.\n";
  print "This is free software: you are free to change and redistribute it.\n";
  print "There is NO WARRANTY, to the extent permitted by law.\n";
  print "\nWritten by Oskar Liljeblad.\n";
  exit;
}

$plugin->nagios_exit(UNKNOWN, "missing --host/-H parameter\n") if !defined $opt{'host'};

my @thresholds;
my @warning_thresholds = split(/,/, $opt{'warning'});
my @critical_thresholds = split(/,/, $opt{'critical'});
$plugin->nagios_exit(UNKNOWN, "must specify equal number of warning and critical thresholds\n") if @warning_thresholds != @critical_thresholds;
if (@warning_thresholds != 0) {
  for (my $c = 0; $c < @warning_thresholds; $c++) {
    $plugin->nagios_exit(UNKNOWN, "invalid warning threshold `$warning_thresholds[$c]'\n") if $warning_thresholds[$c] !~ /^(\d+|\d+:|~:\d+|\d+:\d+|@\d+:\d+)$/;
    $plugin->nagios_exit(UNKNOWN, "invalid critical threshold `$critical_thresholds[$c]'\n") if $critical_thresholds[$c] !~ /^(\d+|\d+:|~:\d+|\d+:\d+|@\d+:\d+)$/;
    push @thresholds, Nagios::Plugin::Threshold->set_thresholds(warning => $warning_thresholds[$c], critical => $critical_thresholds[$c]);
  }
}
$plugin->nagios_exit(UNKNOWN, "surplus operand\n") if @ARGV;

my $retries = 1; # default number of SNMP retries in Net::SNMP
my @session_opts = (
  -hostname => $opt{'host'},
  -version => $opt{'snmp-version'},
  -timeout => int ($opt{'timeout'} / ($retries+1))
);
$opt{'community'} //= 'public' if $opt{'snmp-version'} !~ /^3|snmpv3$/;
push @session_opts, -port => $opt{'port'} if defined $opt{'port'};
push @session_opts, -community => $opt{'community'} if defined $opt{'community'};
push @session_opts, -username => $opt{'username'} if defined $opt{'username'};
push @session_opts, -authpassword => $opt{'auth-password'} if defined $opt{'auth-password'};
push @session_opts, -privpassword => $opt{'priv-password'} if defined $opt{'priv-password'};
push @session_opts, -authprotocol => $opt{'auth-protocol'} if defined $opt{'auth-protocol'};
push @session_opts, -privprotocol => $opt{'priv-protocol'} if defined $opt{'priv-protocol'};
push @session_opts, -maxmsgsize => $opt{'snmp-message-size'} if defined $opt{'snmp-message-size'};
push @session_opts, -localaddr => $opt{'localaddr'} if defined $opt{'localaddr'};
push @session_opts, -localport => $opt{'localport'} if defined $opt{'localport'};
my ($session, $error) = Net::SNMP->session(@session_opts);
$plugin->nagios_exit(UNKNOWN, "cannot open SNMP session: $error\n") if !defined $session;

my $baseoid = '1.3.6.1.4.1.9.9.48.1.1.1'; # ciscoMemoryPoolEntry in CISCO-MEMORY-POOL-MIB
my $result = $session->get_table($baseoid);
$plugin->nagios_exit(UNKNOWN, "cannot get memory pool entry table (CISCO-MEMORY-POOL-MIB::ciscoMemoryPoolEntry): ".$session->error."\n") if !defined $result;

my @indexes;
foreach my $key (keys %{$result}) {
  my ($index) = ($key =~ /^$baseoid\.2\.(.*)$/);
  push @indexes, $index if defined $index && filter_index($opt{'filter'}, $index);
}
$plugin->nagios_exit(UNKNOWN, "no memory pool entries available\n") if !@indexes;

my $status = OK;
my @reports;
my $threshold_index = 0;
foreach my $index (sort @indexes) {
  my $name = $result->{"$baseoid.2.$index"};
  my $used = get_result("$baseoid.5.$index", qr/^\d+$/, "memory used for $name");
  my $free = get_result("$baseoid.6.$index", qr/^\d+$/, "memory free for $name");

  if ($opt{'list'}) {
    print "$index $name\n";
  } else {
    my $used_percent = int($used*100/($used+$free) + 0.5);
    my $item_status = OK;
    my ($threshold, $byte_threshold);
    if (@thresholds > 0) {
      $threshold_index = 0 if $threshold_index >= @thresholds;
      $threshold = $thresholds[$threshold_index++];
      $item_status = $threshold->get_status($used_percent);    
      #$threshold_index++ if $threshold_index < $#threshold_index;
    }
    $status = max($status, $item_status);
    push @reports, [ $item_status, "$name $used_percent\%" ] if $opt{'verbose'} == 0;
    push @reports, [ $item_status, "$name\[$index\] $used_percent\%" ] if $opt{'verbose'} >= 1;
    add_perfdata($name.'_used%', $used_percent, '%', 0, 100, $threshold);
    if (defined $threshold) {
      $byte_threshold = Nagios::Plugin::Threshold->set_thresholds(
        warning => convert_percent_range($threshold->{'warning'}, $used+$free),
        critical => convert_percent_range($threshold->{'critical'}, $used+$free));
      add_perfdata($name.'_used', $used, 'B', 0, $used+$free, $byte_threshold);
    }
  }
}

exit 0 if $opt{'list'};

my @report_critical = map { $_->[1] } grep { $_->[0] == CRITICAL } @reports;
my @report_warning = map { $_->[1] } grep { $_->[0] == WARNING } @reports;
my @report_normal = map { $_->[1] } grep { $_->[0] == OK } @reports;
my $report = 'Memory: ';
if ($status == CRITICAL) {
  $report .= join(', ', @report_critical);
  $report .= ', WARNING: '.join(', ', @report_warning) if @report_warning;
  $report .= ', OK: '.join(', ', @report_normal) if @report_normal;
} elsif ($status == WARNING) {
  $report .= join(', ', @report_warning);
  $report .= ', OK: '.join(', ', @report_normal) if @report_normal;
} elsif ($status == OK) {
  $report .= join(', ', @report_normal);
}
$plugin->nagios_exit($status, $report);

sub convert_percent_range {
  my ($range, $max) = @_;
  if ($range =~ /^\D*(\d+)\D*$/) {
    my $value = int($1*$max/100 + 0.5);
    $range =~ s/(\d+)/$value/;
  } elsif ($range =~ /^\D*(\d+)\D+(\d+)\D*$/) {
    my ($value1, $value2) = map { int($_*$max/100 + 0.5) } ($1, $2);
    $range =~ s/(\d+)(\D+)(\d+)/$value1$2$value2/;
  }
  return $range;
}

sub add_perfdata {
  my ($label, $value, $unit, $min, $max, $threshold) = @_;
  if (!defined $opt{'no-perfdata'}) {
    $unit //= '';
    $max //= '';
    $min //= 0;
    if (defined $threshold) {
      $plugin->add_perfdata(label => $label, value => $value, uom => $unit, min => $min, max => $max, threshold => $threshold);
    } else {
      $plugin->add_perfdata(label => $label, value => $value, uom => $unit, min => $min, max => $max);
    }
  }
}

sub get_result {
  my ($oid, $re, $field) = @_;
  my $value = $result->{$oid};
  $plugin->nagios_exit(UNKNOWN, "missing $field\n") if !defined $value;
  $plugin->nagios_exit(UNKNOWN, "invalid $field: $value\n") if $value !~ $re;
  return $value;
}

sub noun {
  my ($n,$s,$p) = @_;
  return $n == 1 ? $s : $p;
}

sub filter_index {
  my ($filterspec, $index) = @_;
  my $include = 1;
  if (defined $filterspec) {
    return (!defined $1 || $1 eq '+') if $filterspec =~ /^([-+])?all$/;
    $include = 0 if substr($filterspec, 0, 1) ne '-';
    my $inclexcl = 1;
    my $field = 'index';
    my $mode = '=';
    foreach my $item (split_filter_spec($filterspec)) {
      my $filtervalue;
      if ($item =~ /^([+-])?(?:(index)([=:]))?(.*)$/) {
        (my $newinclexcl, my $newfield, my $newmode, $filtervalue) = ($1, $2, $3, $4);
        $inclexcl = ($newinclexcl eq '+' ? 1 : 0) if defined $newinclexcl;
        $mode = $newmode if defined $newmode;
      } else {
        $filtervalue = $item;
      }
      my $value = $index;
      if (defined $value) {
        if ($filtervalue =~ /^(.*)\.\.(.*)$/ && $mode eq '=') {
          my ($firstvalue, $lastvalue) = ($1, $2);
          $include = $inclexcl if generic_cmp($firstvalue, $value) <= 0 && generic_cmp($value, $lastvalue) <= 0;
        } else {
          $include = $inclexcl if $mode eq '=' && $value eq $filtervalue;
          $include = $inclexcl if $mode eq ':' && index(lc $value, lc $filtervalue) >= 0;
        }
      }
    }
  }
  return $include;
}

sub split_filter_spec {
  my ($spec) = @_;
  my @items = ();
  my $item = '';
  for (my $c = 0; $c < length $spec; $c++) {
    if (substr($spec, $c, 2) eq ',,') {
      $item .= ',';
      $c++;
    } elsif (substr($spec, $c, 1) eq ',') {
      push @items, $item;
      $item = '';
    } else {
      $item .= substr($spec, $c, 1);
    }
  }
  push @items, $item if $item ne '';
  return @items;
}

sub generic_cmp {
  my ($a,$b) = @_;
  my @a = split(/(\d+)/, $a);
  my @b = split(/(\d+)/, $b);
  for (my $c = 0; $c < @a && $c < @b; $c++) {
    if ($a[$c] =~ /^\d+$/ && $b[$c] =~ /^\d+$/) {
      my $v = $a[$c] <=> $b[$c];
      return $v if $v != 0;
    } else {
      my $v = $a[$c] cmp $b[$c];
      return $v if $v != 0;
    }
  }
  return @a <=> @b;
}
