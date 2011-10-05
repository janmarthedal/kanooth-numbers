#!/usr/bin/perl

print <<EOF;
/*
 * File:   tests/test_return_types.cpp
 * Author: Jan Marthedal Rasmussen
 *
 * Generated automatically
 *
 * (C) Copyright Jan Marthedal Rasmussen 2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>
#include "test_common.hpp"
#include <kanooth/type_traits.hpp>
#include <kanooth/numbers/number_traits.hpp>
#include <kanooth/numbers/natural_number.hpp>
#include <kanooth/numbers/integer.hpp>
#include <kanooth/numbers/positive_float.hpp>

using kanooth::numbers::natural_number;
using kanooth::numbers::integer;
using kanooth::numbers::positive_float;

int main() {
EOF

my %assigns = ();

sub is_long_long {
    $type = shift;
    return $type eq "long long" || $type eq "unsigned long long";
}

while (<>) {
    if (/^\s*#/) {
        # ignore
    } elsif (/^\s*\*\s*(.*?)\s*$/) {
        @ops = split(/\s*,\s*/, $1);
    } elsif (/^\s*([a-zA-Z_ ]*?)\s*,\s*([a-zA-Z_ ]*?)\s*$/) {
        foreach (@ops) {
            $long_long_guard = is_long_long($1) || is_long_long($2);
            print "#ifdef KANOOTH_HAS_LONG_LONG\n" if $long_long_guard;
            print "  static_assert<kanooth::numbers::${_}_enabled<$1, $2>::enabled>();\n";
            print "#endif\n" if $long_long_guard;
            if ($_ == "set") {
                push(@{$assigns{$2}}, $1);
            }
        }
    } elsif (/^\s*([a-zA-Z_ ]*?)\s*<-\s*([a-zA-Z_ ]*?)\s*,\s*([a-zA-Z_ ]*?)\s*$/) {
        foreach (@ops) {
            $op = $_;
            $long_long_guard = is_long_long($1) || is_long_long($2) || is_long_long($3);
            print "#ifdef KANOOTH_HAS_LONG_LONG\n" if $long_long_guard;
            print "  static_assert_type<$1, typename kanooth::numbers::${op}_result<$2, $3>::type>();\n";
            if (defined $assigns{$1}) {
                foreach (@{$assigns{$1}}) {
                    $long_long_guard2 = is_long_long($_) && !$long_long_guard;
                    print "#ifdef KANOOTH_HAS_LONG_LONG\n" if $long_long_guard2;
                    print "  static_assert<kanooth::numbers::${op}_enabled<${_}, $2, $3>::enabled>();\n";
                    print "#endif\n" if $long_long_guard2;
                }
            } else {
                print "  static_assert<kanooth::numbers::${op}_enabled<$1, $2, $3>::enabled>();\n";
            }
            print "#endif\n" if $long_long_guard;
        }
    }
}

print <<EOF;
  std::cout << "Nothing to see here - if it compiled, the test was successful" << std::endl;
  return 0;
}
EOF
