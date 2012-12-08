#ifndef KANOOTH_NUMBERS_LEAST_SIGNIFICANT_BIT_HPP
#define KANOOTH_NUMBERS_LEAST_SIGNIFICANT_BIT_HPP

#include <kanooth/fixed_width_ints.hpp>

namespace kanooth {
namespace numbers {
        
namespace {

const short ruler_table[256] = {
   -1,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0 };

}

template <typename T>
struct least_significant_bit_eval {
    unsigned operator()(T v) {
        unsigned r = 0;
        while (!(v & 1)) {
            v >>= 1;
            ++r;
        }
        return r;
    }
};

template <>
struct least_significant_bit_eval<uint8_t> {
    inline unsigned operator()(uint8_t v) {
        return ruler_table[v];
    }
};

template <>
struct least_significant_bit_eval<uint16_t> {
    unsigned operator()(uint16_t v) {
        static const uint16_t ones = -1;
        unsigned r = 0;
        if (!(v & (ones >> 8))) { v >>= 8; r += 8; }
        return r + ruler_table[v & 0xff];
    }
};

template <>
struct least_significant_bit_eval<uint32_t> {
    unsigned operator()(uint32_t v) {
        static const uint32_t ones = -1;
        unsigned r = 0;
        if (!(v & (ones >> 16))) { v >>= 16; r += 16; }
        if (!(v & (ones >> 24))) { v >>=  8; r +=  8; }
        return r + ruler_table[v & 0xff];
    }
};

#ifdef KANOOTH_HAS_INT64_T

template <>
struct least_significant_bit_eval<uint64_t> {
    unsigned operator()(uint64_t v) {
        static const uint64_t ones = -1;
        unsigned r = 0;
        if (!(v & (ones >> 32))) { v >>= 32; r += 32; }
        if (!(v & (ones >> 48))) { v >>= 16; r += 16; }
        if (!(v & (ones >> 56))) { v >>=  8; r +=  8; }
        return r + ruler_table[v & 0xff];
    }
};

#endif

#ifdef KANOOTH_HAS_INT128_T

template <>
struct least_significant_bit_eval<uint128_t> {
    unsigned operator()(uint128_t v) {
        static const uint128_t ones = -1;
        unsigned r = 0;
        if (!(v & (ones >>  64))) { v >>= 64; r += 64; }
        if (!(v & (ones >>  96))) { v >>= 32; r += 32; }
        if (!(v & (ones >> 112))) { v >>= 16; r += 16; }
        if (!(v & (ones >> 120))) { v >>=  8; r +=  8; }
        return r + ruler_table[v & 0xff];
    }
};

#endif

template <typename T>
inline unsigned least_significant_bit(T v)
{
    return least_significant_bit_eval<T>()(v);
}

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_LEAST_SIGNIFICANT_BIT_HPP

