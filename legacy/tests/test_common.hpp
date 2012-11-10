/*
 * File:   test/test_common.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2011-09-30 06:32Z
 *
 * (C) Copyright Jan Marthedal Rasmussen 2011
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef _KANOOTH_TEST_COMMON_HPP
#define _KANOOTH_TEST_COMMON_HPP

template <bool> struct static_assert;
template <> struct static_assert<true> {};

template <typename, typename> struct static_assert_type;
template <typename T> struct static_assert_type<T, T> {};

#define KANOOTH_STATIC_ASSERT(P) static_assert<P>()
#define KANOOTH_STATIC_ASSERT_TYPE(S, T) static_assert_type<S, T>()

#endif // _KANOOTH_TEST_COMMON_HPP
