/*
 * File:   lowlevel.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-19 14:39Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#ifndef _LOWLEVEL_HPP
#define _LOWLEVEL_HPP


#include "lowlevel_generic.hpp"

#if 1
#if defined( __GNUC__ ) && ( defined( __i386__ ) || defined( __x86_64__ ) )
#include "lowlevel_gcc_x86.hpp"
#endif
#endif


#endif	/* _LOWLEVEL_HPP */
