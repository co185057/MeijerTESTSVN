#!/usr/bin/perl

#use 5.010;
#use strict;
#use warnings;

# The first line of the script envokes Perl  
# Use "/usr/bin/perl -w" option for debugging

# Scalar variables
# variable declaration
$iTer = 0;
$iNdex = 0;
my @arr1;
# use perl variable ENV assotiative array to read the computer name environment variable
$computername = $ENV{'COMPUTERNAME'};
print "$computername\n"; # Print the coputer name

# use perl variable ENV assotiative array to read the current drive name
$drivename = $ENV{'APP_DRIVE'};
print "$drivename\\SCOT\\IMAGE\\EndOfTrip.png \n";



my $string = $ENV{'COMPUTERNAME'};
my @chars = split("", $string);

# Printing individual character from string
my @list;

while($string) { push @list, chop($string); } #Copy each character into an array

$string = $ENV{'COMPUTERNAME'};
#               + Skip leading characters
#               |        + 1st 4 digits are store id
#               |        |           + Last 3 digits are lane number
#               |        |           |
#               v        v           v 
if ($string =~ /[A-Za-z]*([0-9]{4}).*([0-9]{3})$/)
{
    $StoreNumber = $1; # value of 1st paren expression
    $LaneNumber = $2;  # value of 2nd paren expression
    print "Store: $StoreNumber, Lane: $LaneNumber\n";
}
else
{
    print "Can't parse string\n";
}
 
 my $found = join "", @arr1;  # copy the array value to scalar, arr1 is a Lane Id 
 my $found1 = join "", @arr2; # arr2  is Store number
 my $string_ = '27' . $LaneNumber . '000099900'; # concatenate the strings 
 print "$string_ \n";
 print "$found \n";
 print "$found1 \n";
 
 print "$iTer\n";
 print "$iNdex \n";

# Printing all the system environmental variables
#foreach $key (keys(%ENV)) {

   # printf("%-10.10s: $ENV{$key}\n", $key);
#}

# running an executable file from perl script

my $cmd1 = "C:\\scot\\bin\\MakeQRCodeU.exe";
my $cmd = "notepad.exe";
#my @args = ($cmd, "readme.txt");

print "$cmd1\n";
print "$drivename\\SCOT\\IMAGE\\EndOfTrip.png \n"; 

print "$string_ \n";

system("$cmd1", "$drivename\\SCOT\\IMAGE\\EndOfTrip.png" ,$string_ ,2, 4);


if($? == -1) {
    die "system $prog failed: $?";
}
