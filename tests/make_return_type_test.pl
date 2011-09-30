#!/usr/bin/perl

while (<>) {
    if (/^\s*\*\s*(.*?)\s*$/) {
        print $1;
        @ops = split(/\s*,\s*/, $1);
        print @ops;
    }
}

print;