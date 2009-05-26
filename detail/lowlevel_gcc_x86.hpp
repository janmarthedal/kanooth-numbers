/*
 * File:   lowlevel_gcc_x86.hpp
 * Author: Jan Marthedal Rasmussen
 *
 * Created 2009-05-23 08:25Z
 *
 * (C) Copyright SputSoft 2009
 * Use, modification and distribution are subject to the
 * Boost Software License, Version 1.0. (See accompanying file
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */


#ifndef _LOWLEVEL_GCC_X86_HPP
#define _LOWLEVEL_GCC_X86_HPP


#include <boost/cstdint.hpp>


/*namespace com {
namespace sputsoft {
namespace multiprecision {*/
namespace lowlevel {


typedef boost::uint32_t U32;


/*void foo(const unsigned* ufirst, const unsigned* ulast, const unsigned* vfirst, const unsigned* vlast, unsigned* dst)
{
  __asm__(
      "  xorb  %%ah,%%ah\n"
      "1:cmpl  %%ecx,%%edi\n"
      "  je    2f\n"
      "  sahf\n"
      "  movl  (%%ebx),%%eax\n"
      "  adcl  (%%ecx),%%eax\n"
      "  movl  %%eax,(%%esi)\n"
      "  lahf\n"
      "  addl  $0x4,%%ebx\n"
      "  addl  $0x4,%%ecx\n"
      "  addl  $0x4,%%esi\n"
      "  jmp   1b\n"
      "2:cmpl  %%ebx,%%edx\n"
      "  je    4f\n"
      "  sahf\n"
      "  jnc   3f\n"
      "  movl  (%%ebx),%%eax\n"
      "  incl  %%eax\n"              // Carry not affected?!
      "  movl  %%eax,(%%esi)\n"
      "  lahf\n"
      "  addl  $0x4,%%ebx\n"
      "  addl  $0x4,%%esi\n"
      "  jmp   2b\n"
      "3:cmpl  %%ebx,%%edx\n"
      "  je    4f\n"
      "  movl  (%%ebx),%%eax\n"
      "  movl  %%eax,(%%esi)\n"
      "  addl  $0x4,%%ebx\n"
      "  addl  $0x4,%%esi\n"
      "  jmp   3b\n"
      "4:\n"
      :
      : "b" (ufirst), "c" (vfirst), "S" (dst), "d" (ulast), "D" (vlast)
      : "memory", "eax");
}*/

template <>
inline void double_mult<U32>(const U32 u, const U32 v, U32& low, U32& high)
{
  __asm__ (
      "mull  %%ebx"
      : "=d" (high), "=a" (low)
      : "a" (u), "b" (v));
}

template <>
inline void double_mult_add_add<U32>(const U32 u, const U32 v, const U32 a1, const U32 a2, U32& low, U32& high)
{
  __asm__ (
      "mull  %%ebx\n"
      "addl  %%ecx,%%eax\n"
      "adcl  $0,%%edx\n"
      "addl  %%esi,%%eax\n"
      "adcl  $0,%%edx\n"
      : "=d" (high), "=a" (low)
      : "a" (u), "b" (v), "c" (a1), "S" (a2));
}

template <>
inline void double_div<U32>(U32 uhigh, U32 ulow, U32 v, U32& quot, U32& rem)
{
  __asm__ ("divl %%ebx"
           : "=a" (quot), "=d" (rem)
           : "d" (uhigh), "a" (ulow), "b" (v));
}


} // lowlevel
/*} // multiprecision
} // sputsoft
} // com*/

#endif	/* _LOWLEVEL_GCC_X86_HPP */
