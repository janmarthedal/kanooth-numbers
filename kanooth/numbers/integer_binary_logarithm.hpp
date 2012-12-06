#ifndef KANOOTH_NUMBERS_INTEGER_BINARY_LOGARITHM_HPP
#define KANOOTH_NUMBERS_INTEGER_BINARY_LOGARITHM_HPP

#include <kanooth/fixed_width_ints.hpp>

namespace kanooth {
namespace numbers {
        
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

template <typename T>
struct integer_binary_logarithm_eval {
    unsigned operator()(T v) {
        unsigned r = -1;
        while (v) {
            v >>= 1;
            ++r;
        }
        return r;
    }
};

template <>
struct integer_binary_logarithm_eval<uint8_t> {
    unsigned operator()(uint8_t v) {
        return int_bin_log_table[v];
    }
};

template <>
struct integer_binary_logarithm_eval<uint16_t> {
    unsigned operator()(uint16_t v) {
        static const uint16_t ones = -1;
        unsigned r = 0;
        if (v & (ones << 8)) { v >>= 8; r += 8; }
        return r + int_bin_log_table[v];
    }
};

template <>
struct integer_binary_logarithm_eval<uint32_t> {
    unsigned operator()(uint32_t v) {
        static const uint32_t ones = -1;
        unsigned r = 0;
        if (v & (ones << 16)) { v >>= 16; r += 16; }
        if (v & (ones <<  8)) { v >>=  8; r +=  8; }
        return r + int_bin_log_table[v];
    }
};

#ifdef KANOOTH_HAS_INT64_T

template <>
struct integer_binary_logarithm_eval<uint64_t> {
    unsigned operator()(uint64_t v) {
        static const uint64_t ones = -1;
        unsigned r = 0;
        if (v & (ones << 32)) { v >>= 32; r += 32; }
        if (v & (ones << 16)) { v >>= 16; r += 16; }
        if (v & (ones <<  8)) { v >>=  8; r +=  8; }
        return r + int_bin_log_table[v];
    }
};

#endif

#ifdef KANOOTH_HAS_INT128_T

template <>
struct integer_binary_logarithm_eval<uint128_t> {
    unsigned operator()(uint128_t v) {
        static const uint128_t ones = -1;
        unsigned r = 0;
        if (v & (ones << 64)) { v >>= 64; r += 64; }
        if (v & (ones << 32)) { v >>= 32; r += 32; }
        if (v & (ones << 16)) { v >>= 16; r += 16; }
        if (v & (ones <<  8)) { v >>=  8; r +=  8; }
        return r + int_bin_log_table[v];
    }
};

#endif

template <typename T>
unsigned integer_binary_logarithm(T v)
{
    return integer_binary_logarithm_eval<T>()(v);
}

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_INTEGER_BINARY_LOGARITHM_HPP

