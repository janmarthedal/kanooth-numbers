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

int main() {
EOF

while (<>) {
    if (/^\s*\*\s*(.*?)\s*$/) {
        @ops = split(/\s*,\s*/, $1);
    } elsif (/^\s*(.*?)\s*,\s*(.*?)\s*->\s*(.*?)\s*$/) {
        foreach (@ops) {
            print "  static_assert_type<typename kanooth::numbers::${_}_result<$1, $2>::type, $3>();\n";
        }
    }
}

print <<EOF;
  std::cout << "Nothing to see here - if it compiled, the test was successful" << std::endl;
  return 0;
}
EOF
