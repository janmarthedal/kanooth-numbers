#ifndef KANOOTH_NUMBERS_INTEGER_BINARY_LOGARITHM_HPP
#define KANOOTH_NUMBERS_INTEGER_BINARY_LOGARITHM_HPP

#include <stdint.h>

namespace {

const short int_bin_log_table[256] = {
 -1, 0, 1,1, 2,2,2,2, 3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7 };

}

unsigned integer_binary_logarithm(uint8_t v) {
  return int_bin_log_table[v];
}

unsigned integer_binary_logarithm(uint16_t v) {
  static const uint16_t ones = -1;
  unsigned r = 0;
  if (v & (ones << 8)) { v >>= 8; r += 8; }
  return r + int_bin_log_table[v];
}

unsigned integer_binary_logarithm(uint32_t v) {
  static const uint32_t ones = -1;
  unsigned r = 0;
  if (v & (ones << 16)) { v >>= 16; r += 16; }
  if (v & (ones <<  8)) { v >>=  8; r +=  8; }
  return r + int_bin_log_table[v];
}

#endif // KANOOTH_NUMBERS_INTEGER_BINARY_LOGARITHM_HPP

