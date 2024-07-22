#!/usr/local/bin/perl

use warnings;
use strict;

use Device::SerialPort;

# use Time::HiRes qw(usleep);

my $cmdlinechipname = $ARGV[0];
if ( not defined( $cmdlinechipname ) ) { die( "No chip name spcified!\n") };
my ($sec,$min,$hour,$mday,$mon,$year,$wday, $yday, $isdst) = localtime;
$cmdlinechipname .= "-" . $sec . "_" . $min . "_" . $hour . "-" . $mday . "_" . $mon . "_" . $year;
# mkdir( $cmdlinechipname ) or die("Cannot create output directory: " . $cmdlinechipname . "\n" );
mkdir( $cmdlinechipname );
mkdir( $cmdlinechipname . "/data" );
mkdir( $cmdlinechipname . "/graphs" );

my $cmdlinefilename = $ARGV[1];
if ( not defined( $cmdlinefilename ) ) { 
    print( "No port specified, using: /dev/cu.usbserial-14130\n");
    $cmdlinefilename = "/dev/cu.usbserial-14130";
 };

my $serialporthandle = Device::SerialPort->new( $cmdlinefilename );

if ( not defined( $serialporthandle ) ) {
    die( "Could not open serial port located at:" . $cmdlinefilename . "\n" );
}

$serialporthandle->baudrate(115200);
$serialporthandle->databits(8);
$serialporthandle->parity("none");
$serialporthandle->stopbits(1);

sub waitforcontent {
    my $searchstring = shift;
    my $rawinputbuffer = "";
    my $loopvar = 1;
    my $inputcounter = 0;

    while ( $loopvar ) {

        my $tempvar = $serialporthandle->read(255);

        if ( $tempvar ) {

            $inputcounter += 1;
            if ( $inputcounter > 1024 ) {
                $inputcounter = 0;
                print ".";
                select()->flush();
            }

            $rawinputbuffer .= $tempvar;

            if ( substr( $rawinputbuffer , -64 ) =~ /$searchstring/) {

                $loopvar = 0;
                print "ok!\n";
            }

        }

    }

    return $rawinputbuffer;
}


print "Waiting for uno to report ready.\n";

waitforcontent("Ready: ");

print "Running all tests on chip. Please wait.\n";

$serialporthandle->write("\n");

my $rawresultsvar = waitforcontent("Ready: ");

print "Tests completed.\n";

print "received " . length( $rawresultsvar ) . " bytes\n";



