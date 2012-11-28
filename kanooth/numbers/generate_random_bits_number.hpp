#ifndef KANOOTH_NUMBERS_GENERATE_RANDOM_BITS_NUMBER_HPP
#define KANOOTH_NUMBERS_GENERATE_RANDOM_BITS_NUMBER_HPP

#include <kanooth/make_unsigned.hpp>
#include <kanooth/numbers/integer_binary_logarithm.hpp>

namespace kanooth {
namespace numbers {

namespace {
    template <typename T>
    typename make_unsigned<T>::type subtract_to_unsigned(T x, T y)
    {
        typedef typename make_unsigned<T>::type unsigned_type;
        if (y < 0)
            return unsigned_type(x) + unsigned_type(-(y+1)) + 1;
        return unsigned_type(x - y);
    }
}

template <typename T, typename Engine>
T generate_random_bits_number(Engine& engine, unsigned width)
{
    typedef typename Engine::result_type base_type;
    typedef typename make_unsigned<base_type>::type unsigned_base;
    unsigned_base range = subtract_to_unsigned(Engine::max(), Engine::min());    
    unsigned_base limit, value;
    unsigned base_bits;
    T result = T();
    
    if (range == unsigned_base(-1)) {
        base_bits = std::numeric_limits<unsigned_base>::digits;
        limit = unsigned_base(-1);
    } else {
        base_bits = integer_binary_logarithm(range + 1);
        limit = (unsigned_base(1) << base_bits) - 1;
    }
    
    while (width >= base_bits) {
        do {
            value = subtract_to_unsigned(engine(), Engine::min());
        } while (value > limit);
        result <<= base_bits;
        result |= value;
        width -= base_bits;
    }
    if (width) {
        do {
            value = subtract_to_unsigned(engine(), Engine::min());
        } while (value > limit);
        result <<= width;
        result |= value & ((unsigned_base(1) << width) - 1);
    }

    return result;
}

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_GENERATE_RANDOM_BITS_NUMBER_HPP
