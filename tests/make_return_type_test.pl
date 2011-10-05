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

while (<>) {
    if (/^\s*#/) {
        # ignore
    } elsif (/^\s*\*\s*(.*?)\s*$/) {
        @ops = split(/\s*,\s*/, $1);
    } elsif (/^\s*([a-zA-Z_ ]*?)\s*,\s*([a-zA-Z_ ]*?)\s*$/) {
        foreach (@ops) {
            print "  static_assert<kanooth::numbers::${_}_enabled<$1, $2>::enabled>();\n";
            if ($_ == "set") {
                push(@{$assigns{$2}}, $1);
            }
        }        
    } elsif (/^\s*([a-zA-Z_ ]*?)\s*<-\s*([a-zA-Z_ ]*?)\s*,\s*([a-zA-Z_ ]*?)\s*$/) {
        foreach (@ops) {
            $op = $_;
            print "  static_assert_type<$1, typename kanooth::numbers::${op}_result<$2, $3>::type>();\n";
            if (defined $assigns{$1}) {
                foreach (@{$assigns{$1}}) {
                    print "  static_assert<kanooth::numbers::${op}_enabled<${_}, $2, $3>::enabled>();\n";                
                }
            } else {
                print "  static_assert<kanooth::numbers::${op}_enabled<$1, $2, $3>::enabled>();\n";
            }
        }
    }
}

print <<EOF;
  std::cout << "Nothing to see here - if it compiled, the test was successful" << std::endl;
  return 0;
}
EOF
