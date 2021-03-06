#ifndef KANOOTH_NUMBERS_NATURAL_NUMBER_BASE_HPP
#define KANOOTH_NUMBERS_NATURAL_NUMBER_BASE_HPP

#include <memory>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cstring>  // strlen

#include <kanooth/number_bits.hpp>
#include <kanooth/numbers/integer_binary_logarithm.hpp>
#include <kanooth/numbers/least_significant_bit.hpp>

namespace kanooth {
namespace numbers {

namespace {

    template <typename T, unsigned N>
    struct pow10 {
        static const T value = T(10) * pow10<T, N-1>::value;
    };

    template <typename T>
    struct pow10<T, 0> {
        static const T value = 1;
    };

    template <typename T>
    T ceil_div(T a, T b)
    {
        return (a + b - 1) / b;
    }

}

template <typename LowLevel, typename Allocator = std::allocator<void> >
class natural_number_base : private Allocator::template rebind<typename LowLevel::digit_type>::other
{

    typedef typename LowLevel::digit_type digit_type;
    typedef typename Allocator::template rebind<digit_type>::other allocator_type;
    typedef typename allocator_type::size_type size_type;
    static const unsigned digit_bits = ::kanooth::number_bits<digit_type>::value;
    static const unsigned digit_decimals = ::std::numeric_limits<digit_type>::digits10;
    static const digit_type digit_largest_pow10 = pow10<digit_type, ::std::numeric_limits<digit_type>::digits10>::value;

public:

    natural_number_base()
    {
        clear();
    }

    natural_number_base(const natural_number_base& other)
    {
        if (other.digits) {
            allocate(other.digits);
            digits = other.digits;
            LowLevel::copy_forward(digit_array, other.digit_array, other.digits);
        } else {
            clear();
        }
    }

#if !defined(KANOOTH_NO_RVALUE_REFS)
    natural_number_base(natural_number_base&& other)
            : digits(other.digits), allocated(other.allocated), digit_array(other.digit_array)
    {
        other.clear();
    }
#endif

    natural_number_base(const char* s) : digits(0)
    {
        // max_bits = floor(decimals*log(10)/log(2)) + 1 <= floor(decimals*10/3) + 1
        unsigned length = std::strlen(s);
        const unsigned max_digits = (10 * length / 3) / digit_bits + 1;
        unsigned chunk_size;
        digit_type chunk_value;
        allocate(max_digits + 1);  // extra digit for multiplying
        while (length >= digit_decimals) {
            chunk_value = 0;
            for (chunk_size = digit_decimals; chunk_size != 0; --chunk_size) {
                chunk_value = 10*chunk_value + (*s - '0');
                ++s;
            }
            multiply_digit(*this, digit_largest_pow10);
            add_digit(*this, chunk_value);
            length -= digit_decimals;
        }
        if (length) {
            digit_type pow10 = 1;
            chunk_value = 0;
            while (length--) {
                chunk_value = 10*chunk_value + (*s - '0');
                pow10 *= 10;
                ++s;
            }
            multiply_digit(*this, pow10);
            add_digit(*this, chunk_value);
        }
    }

    natural_number_base& operator=(const natural_number_base& other)
    {
        if (this != &other) {
            if (size_ok(other.digits)) {
                LowLevel::copy_forward(digit_array, other.digit_array, other.digits);
                digits = other.digits;
            } else {
                natural_number_base(other).swap(*this);
            }
        }
        return *this;
    }

#if !defined(KANOOTH_NO_RVALUE_REFS)
    natural_number_base& operator=(natural_number_base&& other)
    {
        other.swap(*this);
        return *this;
    }
#endif

    natural_number_base& operator=(const char* s)
    {
        natural_number_base(s).swap(*this);
        return *this;
    }

    ~natural_number_base()
    {
        if (allocated) {
            allocator().deallocate(digit_array, allocated);
        }
    }

    void swap(natural_number_base& other)
    {
        std::swap(digit_array, other.digit_array);
        std::swap(digits, other.digits);
        std::swap(allocated, other.allocated);
    }

    bool is_zero() const
    {
        return digits == 0;
    }

    void add(const natural_number_base& a, const natural_number_base& b)
    {
        // these checks for zero are not necessary, but an optimization
        if (a.is_zero()) {
            *this = b;
        } else if (b.is_zero()) {
            *this = a;
        } else {
            size_type max_digits = std::max(a.digits, b.digits) + 1;
            if (allocated < max_digits) {
                natural_number_base other(max_digits, digit_unit);
                other.add_number(a, b);
                swap(other);
            } else {
                add_number(a, b);
            }
        }
    }

    // result is undefined if a > b, but no crash
    void subtract(const natural_number_base& a, const natural_number_base& b)
    {
        size_type max_digits = a.digits;
        if (allocated < max_digits) {
            natural_number_base other(max_digits, digit_unit);
            other.subtract_number(a, b);
            swap(other);
        } else {
            subtract_number(a, b);
        }
    }

    void multiply(const natural_number_base& a, const natural_number_base& b)
    {
        if (a.is_zero() || b.is_zero()) {
            *this = 0lu;
        } else {
            size_type res_digits = a.digits + b.digits;
            if (allocated < res_digits || this == &a || this == &b) {
                natural_number_base other(res_digits, digit_unit);
                other.multiply_number(a, b);
                swap(other);
            } else {
                multiply_number(a, b);
            }
        }
    }

    void divide(const natural_number_base& a, const natural_number_base& b)
    {
        natural_number_base r(a.digits + 1);
        quotrem(*this, r, a, b);
    }

    void modulus(const natural_number_base& a, const natural_number_base& b)
    {
        natural_number_base q(a.digits - b.digits + 1);
        quotrem(q, *this, a, b);
    }

    static void quotrem(natural_number_base& q, natural_number_base& r, const natural_number_base& a, const natural_number_base& b)
    {
        if (&q == &r)
            throw std::invalid_argument("q must be different from r");
        if (b.is_zero())
            throw std::overflow_error("division by zero");
        else if (a.digits < b.digits) {
            q = 0lu;
            r = a;
        } else if (&a == &b) {  // necessary?
            q = 1lu;
            r = 0lu;
        } else if (b.digits == 1) {
            r.set_to_int(q.quotrem_digit(a, b.digit_array[0]));
        } else {
            unsigned shift = digit_bits - 1 - integer_binary_logarithm(b.digit_array[b.digits-1]);
            size_type r_res_digits = a.digits + 1;  // note: auxiliary digit
            if (r.size_ok(r_res_digits) && (&r != &b || shift != 0)) {
                quotrem_number_step1(q, r, a, b, shift);
            } else {
                natural_number_base rem(r_res_digits, digit_unit);
                quotrem_number_step1(q, rem, a, b, shift);
                rem.swap(r);
            }
        }
    }

    void bitwise_and(const natural_number_base& a, const natural_number_base& b)
    {
        size_type res_digits = std::min(a.digits, b.digits);
        if (allocated < res_digits) {
            natural_number_base tmp(res_digits, digit_unit);
            tmp.bitwise_and_helper(a, b);
            swap(tmp);
        } else {
            bitwise_and_helper(a, b);
        }
    }

    void bitwise_or(const natural_number_base& a, const natural_number_base& b)
    {
        size_type res_digits = std::max(a.digits, b.digits);
        if (allocated < res_digits) {
            natural_number_base tmp(res_digits, digit_unit);
            tmp.bitwise_or_step1(a, b);
            swap(tmp);
        } else {
            bitwise_or_step1(a, b);
        }
    }

    void bitwise_xor(const natural_number_base& a, const natural_number_base& b)
    {
        size_type res_digits = std::max(a.digits, b.digits);
        if (allocated < res_digits) {
            natural_number_base tmp(res_digits, digit_unit);
            tmp.bitwise_xor_step1(a, b);
            swap(tmp);
        } else {
            bitwise_xor_step1(a, b);
        }
    }

    void bitwise_and_not(const natural_number_base& a, const natural_number_base& b)
    {
        size_type res_digits = a.digits;
        if (allocated < res_digits) {
            natural_number_base tmp(res_digits, digit_unit);
            tmp.bitwise_and_not_helper(a, b);
            swap(tmp);
        } else {
            bitwise_and_not_helper(a, b);
        }
    }

    void left_shift(const natural_number_base& a, unsigned long count)
    {
        if (a.is_zero())
            *this = 0ul;
        else if (!count)
            *this = a;
        else {
            size_type res_digits = a.digits + count/digit_bits;
            if (a.digit_array[a.digits-1] >> (digit_bits - (count % digit_bits)))
                ++res_digits;
            if (allocated < res_digits) {
                natural_number_base tmp(res_digits, digit_unit);
                tmp.left_shift_helper(a, count);
                swap(tmp);
            } else {
                left_shift_helper(a, count);
            }
        }
    }

    void right_shift(const natural_number_base& a, unsigned long count)
    {
        if (a.is_zero() || count >= a.digits*digit_bits)
            *this = 0ul;
        else if (!count)
            *this = a;
        else {
            size_type res_digits = a.digits - count/digit_bits;
            if (allocated < res_digits) {
                natural_number_base tmp(res_digits, digit_unit);
                tmp.right_shift_helper(a, count);
                swap(tmp);
            } else {
                right_shift_helper(a, count);
            }
        }
    }

    void bit_set(size_type pos)
    {
        size_type index = pos / digit_bits;
        unsigned offset = pos % digit_bits;
        if (index >= allocated) {
            natural_number_base tmp(index+1, digit_unit);
            LowLevel::copy_forward(tmp.digit_array, digit_array, digits);
            tmp.digits = digits;
            swap(tmp);
        }
        if (index >= digits) {
            LowLevel::fill_zero(digit_array + digits, index + 1 - digits);
            digits = index + 1;
        }
        digit_array[index] |= digit_type(1) << offset;
    }

    void bit_unset(size_type pos)
    {
        size_type index = pos / digit_bits;
        unsigned offset = pos % digit_bits;
        if (index < digits) {
            digit_array[index] &= ~(digit_type(1) << offset);
        }
    }

    bool bit_test(size_type pos) const
    {
        size_type index = pos / digit_bits;
        unsigned offset = pos % digit_bits;
        return (index < digits) && (digit_array[index] & (digit_type(1) << offset));
    }

    void bit_flip(size_type pos)
    {
        size_type index = pos / digit_bits;
        if (index >= digits) {
            bit_set(pos);
        } else {
            unsigned offset = pos % digit_bits;
            digit_array[index] ^= digit_type(1) << offset;
        }
    }

    int compare(const natural_number_base& a) const
    {
        if (digits < a.digits) return -1;
        if (digits > a.digits) return 1;
        return LowLevel::comp(digit_array, a.digit_array, digits);
    }

    unsigned lsb() const
    {
        if (is_zero()) {
            return -1;
        }
        const digit_type* dp = digit_array;
        while (!*dp) {
            ++dp;
        }
        return (dp - digit_array) * digit_bits + least_significant_bit(*dp);
    }

    inline unsigned binary_logarithm() const
    {
        if (digits == 0) {
            return -1;
        }
        return integer_binary_logarithm(digit_array[digits - 1]) + digit_bits * (digits - 1);
    }

    void gcd(const natural_number_base& a, const natural_number_base& b)
    {
        if (a.is_zero()) {
            *this = b;
        } else if (b.is_zero()) {
            *this = a;
        } else {
            natural_number_base u, v;

            unsigned us = a.lsb();
            unsigned vs = b.lsb();
            unsigned shift = (std::min)(us, vs);
            u.right_shift(a, us);
            v.right_shift(b, vs);

            while (true) {
                int c = u.compare(v);
                if (c == 0) break;
                if (c > 0) u.swap(v);
                v.subtract(v, u);
                vs = v.lsb();
                v.right_shift(v, vs);
            }

            left_shift(u, shift);
        }
    }

    std::string str(std::streamsize /*digits*/, std::ios_base::fmtflags) const
    {
        if (is_zero())
            return "0";
        unsigned max_decimals = (digits * digit_bits) / 3 + 1;
        char chars[max_decimals];
        char *end = chars + max_decimals;
        char *begin = end;
        natural_number_base n = *this;
        while (true) {
            digit_type r = n.quotrem_digit(n, digit_largest_pow10);
            if (n.is_zero()) {
                while (r) {
                    *--begin = (r % 10) + '0';
                    r /= 10;
                }
                break;
            } else {
                for (unsigned k = digit_decimals; k; --k) {
                    *--begin = (r % 10) + '0';
                    r /= 10;
                }
            }
        }
        return std::string(begin, end);
    }

    std::string str() const
    {
        return str(0, std::ios_base::dec);
    }

    // *********************************************
    // unsigned rhs
    // *********************************************

    natural_number_base(unsigned v)
    {
        construct_from_int(v);
    }

    natural_number_base& operator=(unsigned v)
    {
        return set_to_int(v);
    }

    inline void convert_to(unsigned* result) const
    {
        *result = value_as<unsigned>();
    }

    inline int compare(unsigned u) const
    {
        return compare_int(u);
    }

    inline void add(const natural_number_base& a, unsigned b)
    {
        add_int(a, b);
    }

    inline void subtract(const natural_number_base& a, unsigned b)
    {
        subtract_int(a, b);
    }

    inline void multiply(const natural_number_base& a, unsigned b)
    {
        multiply_int(a, b);
    }

    inline void divide(const natural_number_base& a, unsigned b)
    {
        quotrem_int(a, b);
    }

    inline void modulus(const natural_number_base& a, unsigned b)
    {
        *this = integer_modulus(a, b);
    }

    inline unsigned quotrem(const natural_number_base& a, unsigned b)
    {
        return quotrem_int(a, b);
    }

    inline static unsigned integer_modulus(const natural_number_base& a, unsigned b)
    {
        natural_number_base q(a.digits, digit_unit);
        return q.quotrem_int(a, b);
    }

    inline static unsigned integer_bitwise_and(const natural_number_base& a, unsigned b)
    {
        return integer_bitwise_and_int(a, b);
    }

    // *********************************************
    // unsigned long rhs
    // *********************************************

    natural_number_base(unsigned long v)
    {
        construct_from_int(v);
    }

    natural_number_base& operator=(unsigned long v)
    {
        return set_to_int(v);
    }

    inline void convert_to(unsigned long* result) const
    {
        *result = value_as<unsigned long>();
    }

    inline int compare(unsigned long u) const
    {
        return compare_int(u);
    }

    inline void add(const natural_number_base& a, unsigned long b)
    {
        add_int(a, b);
    }

    inline void subtract(const natural_number_base& a, unsigned long b)
    {
        subtract_int(a, b);
    }

    inline void multiply(const natural_number_base& a, unsigned long b)
    {
        multiply_int(a, b);
    }

    inline void divide(const natural_number_base& a, unsigned long b)
    {
        quotrem_int(a, b);
    }

    inline void modulus(const natural_number_base& a, unsigned long b)
    {
        *this = integer_modulus(a, b);
    }

    inline unsigned long quotrem(const natural_number_base& a, unsigned long b)
    {
        return quotrem_int(a, b);
    }

    inline static unsigned long integer_modulus(const natural_number_base& a, unsigned long b)
    {
        natural_number_base q(a.digits, digit_unit);
        return q.quotrem_int(a, b);
    }

    inline static unsigned long integer_bitwise_and(const natural_number_base& a, unsigned long b)
    {
        return integer_bitwise_and_int(a, b);
    }

#ifdef KANOOTH_HAS_LONG_LONG

    // *********************************************
    // unsigned long long rhs
    // *********************************************

    natural_number_base(unsigned long long v)
    {
        construct_from_int(v);
    }

    natural_number_base& operator=(unsigned long long v)
    {
        return set_to_int(v);
    }

    inline void convert_to(unsigned long long* result) const
    {
        *result = value_as<unsigned long long>();
    }


    inline int compare(unsigned long long u) const
    {
        return compare_int(u);
    }

    inline void add(const natural_number_base& a, unsigned long long b)
    {
        add_int(a, b);
    }

    inline void subtract(const natural_number_base& a, unsigned long long b)
    {
        subtract_int(a, b);
    }

    inline void multiply(const natural_number_base& a, unsigned long long b)
    {
        multiply_int(a, b);
    }

    inline void divide(const natural_number_base& a, unsigned long long b)
    {
        quotrem_int(a, b);
    }

    inline void modulus(const natural_number_base& a, unsigned long long b)
    {
        *this = integer_modulus(a, b);
    }

    inline unsigned long long quotrem(const natural_number_base& a, unsigned long long b)
    {
        return quotrem_int(a, b);
    }

    inline static unsigned long long integer_modulus(const natural_number_base& a, unsigned long long b)
    {
        natural_number_base q(a.digits, digit_unit);
        return q.quotrem_int(a, b);
    }

    inline static unsigned long long integer_bitwise_and(const natural_number_base& a, unsigned long long b)
    {
        return integer_bitwise_and_int(a, b);
    }

#endif

private:

    enum size_request
    {
        digit_unit
    };

    natural_number_base(size_type min_size, size_request) : digits(0)
    {
        assert(min_size >= 1);
        allocate(min_size);
    }

    static digit_type digit_max()
    {
        return std::numeric_limits<digit_type>::max();
    }

    template <typename T>
    void construct_from_int(T v)
    {
        if (!v) {
            digits = allocated = 0;
            digit_array = 0;
        } else if (sizeof(T) <= sizeof(digit_type) || v <= digit_max()) {
            allocate(1);
            digits = 1;
            digit_array[0] = v;
        } else {
            allocate(ceil_div(sizeof(T), sizeof(digit_type)));
            digits = 0;
            while (v) {
                digit_array[digits++] = v;
                v >>= digit_bits;
            }
        }
    }

    template <typename T>
    T value_as() const {
        if (digits == 0) {
            return 0;
        } else if (digits > 1 && sizeof(T) > sizeof(digit_type)) {
            T r = 0;
            const unsigned max = ceil_div(sizeof(T), sizeof(digit_type));
            unsigned n = 0;
            while (n < digits && n < max) {
                r |= static_cast<T>(digit_array[n]) << (n * digit_bits);
                ++n;
            }
            return r;
        } else {
            return digit_array[0];
        }
    }

    // *******************************************************
    // template helpers for integer rhs
    // *******************************************************

    template <typename T>
    void add_int(const natural_number_base& a, T b)
    {
        // these checks for zero are not necessary, but an optimization
        if (a.is_zero()) {
            *this = b;
        } else if (b == 0) {
            *this = a;
        } else {
            if (sizeof(T) <= sizeof(digit_type) || b <= digit_max()) {
                size_type max_digits = a.digits + 1;
                if (allocated < max_digits) {
                    natural_number_base other(max_digits, digit_unit);
                    other.add_digit(a, b);
                    swap(other);
                } else {
                    add_digit(a, b);
                }
            } else {
                natural_number_base t(b);
                add(a, t);
            }
        }
    }

    template <typename T>
    void subtract_int(const natural_number_base& a, T b)
    {
        if (sizeof(T) <= sizeof(digit_type) || b <= digit_max()) {
            size_type max_digits = a.digits;
            if (allocated < max_digits) {
                natural_number_base other(max_digits, digit_unit);
                other.subtract_digit(a, b);
                swap(other);
            } else {
                subtract_digit(a, b);
            }
        } else {
            natural_number_base t(b);
            subtract(a, t);
        }
    }

    template <typename T>
    void multiply_int(const natural_number_base& a, T b)
    {
        if (sizeof(T) <= sizeof(digit_type) || b <= digit_max()) {
            size_type res_digits = a.digits + 1;
            if (allocated < res_digits) {
                natural_number_base other(res_digits, digit_unit);
                other.multiply_digit(a, b);
                swap(other);
            } else {
                multiply_digit(a, b);
            }
        } else {
            natural_number_base t(b);
            multiply(a, t);
        }
    }

    template <typename T>
    T quotrem_int(const natural_number_base& a, T b)
    {
        if (!b)
            throw std::overflow_error("division by zero");
        else if (a.is_zero()) {
            *this = 0lu;
            return b;
        } else if (sizeof(T) <= sizeof(digit_type) || b <= digit_max()) {
            return quotrem_digit(a, b);
        } else {
            natural_number_base r;
            quotrem(*this, r, a, natural_number_base(b));
            return r.value_as<T>();
        }
    }

    template <typename T>
    static T integer_bitwise_and_int(const natural_number_base& a, T b)
    {
        if (a.is_zero()) {
            return 0;
        } else if (a.digits > 1 && sizeof(T) > sizeof(digit_type)) {
            natural_number_base r;
            r.bitwise_and(a, natural_number_base(b));
            return r.value_as<T>();
        } else {
            return a.digit_array[0] & b;
        }
    }

    // **********************

    inline bool size_ok(size_type min_digits) const
    {
        return allocated >= min_digits;
    }

    inline void set_digits_1(size_type max_digits)
    {
        assert(max_digits >= 1);
        digits = max_digits;
        if (!digit_array[digits-1])
            --digits;
    }

    inline void set_digits_n(size_type max_digits)
    {
        digits = max_digits;
        while (digits && !digit_array[digits-1])
            --digits;
    }

    inline void set_digits_carry(size_type base_digits, digit_type carry)
    {
        digits = base_digits;
        if (carry)
            digit_array[digits++] = carry;
    }

    template <typename T>
    int compare_int(T u) const
    {
        if (sizeof(T) <= sizeof(digit_type) || u <= digit_max()) {
            if (is_zero())
                return u == 0 ? 0 : -1;
            if (digits > 1)
                return 1;
            return digit_array[0] < u ? -1 : digit_array[0] > u ? 1 : 0;
        } else {
            natural_number_base t(u);
            return compare(t);
        }
    }

    template <typename T>
    natural_number_base& set_to_int(T v)
    {
        if ((sizeof(T) <= sizeof(digit_type) || v <= digit_max()) && size_ok(1)) {
            digit_array[0] = v;
            digits = v ? 1 : 0;
        } else {
            natural_number_base(v).swap(*this);
        }
        return *this;
    }

    // we need specialization because constructor from digit_type may not be defined
    natural_number_base& set_to_int(digit_type v)
    {
        if (size_ok(1)) {
            digit_array[0] = v;
            digits = v ? 1 : 0;
        } else {
            natural_number_base t(1, digit_unit);
            t.digit_array[0] = v;
            t.digits = v ? 1 : 0;
            swap(t);
        }
        return *this;
    }

    void add_number(const natural_number_base& a, const natural_number_base& b)
    {
        if (a.digits >= b.digits) {
            digit_type carry = LowLevel::add(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
            set_digits_carry(a.digits, carry);
        } else {
            digit_type carry = LowLevel::add(digit_array, b.digit_array, b.digits, a.digit_array, a.digits);
            set_digits_carry(b.digits, carry);
        }
    }

    void add_digit(const natural_number_base& a, digit_type b)
    {
        digit_type carry = LowLevel::add_1(digit_array, a.digit_array, a.digits, b);
        set_digits_carry(a.digits, carry);
    }

    void subtract_number(const natural_number_base& a, const natural_number_base& b)
    {
        LowLevel::sub(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
        set_digits_n(a.digits);
    }

    void subtract_digit(const natural_number_base& a, digit_type b)
    {
        LowLevel::sub_1(digit_array, a.digit_array, a.digits, b);
        set_digits_n(a.digits);
    }

    void multiply_number(const natural_number_base& a, const natural_number_base& b)
    {
        if (a.digits >= b.digits) {
            LowLevel::mul(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
        } else {
            LowLevel::mul(digit_array, b.digit_array, b.digits, a.digit_array, a.digits);
        }
        set_digits_1(a.digits + b.digits);
    }

    void multiply_digit(const natural_number_base& a, digit_type b)
    {
        if (b) {
            digit_type carry = LowLevel::mul_1(digit_array, a.digit_array, a.digits, b);
            set_digits_carry(a.digits, carry);
        } else {
            digits = 0;
        }
    }

    static void quotrem_number_step1(natural_number_base& q, natural_number_base& rem, const natural_number_base& a, const natural_number_base& b, unsigned shift)
    {
        assert(rem.allocated >= a.digits + 1);
        assert(&rem != &b || shift != 0);  // &rem == &b is ok when shift != 0 since a temporary instead of b will be used as denominator
        rem.digits = a.digits;
        if (shift) {
            natural_number_base denom(b.digits, digit_unit);
            rem.digit_array[a.digits] = LowLevel::lshift(rem.digit_array, a.digit_array, a.digits, shift);
            LowLevel::lshift(denom.digit_array, b.digit_array, b.digits, shift);
            denom.digits = b.digits;
            quotrem_number_step2(q, rem, denom, shift);
        } else {
            // the following will then do nothing if &rem == &a
            LowLevel::copy_forward(rem.digit_array, a.digit_array, a.digits);
            rem.digit_array[a.digits] = 0;
            quotrem_number_step2(q, rem, b, 0);
        }
    }

    static void quotrem_number_step2(natural_number_base& quot, natural_number_base& num, const natural_number_base& denom, unsigned shift)
    {
        assert(denom.digit_array[denom.digits-1] & (digit_type(1) << (digit_bits - 1)));  // denominator normalized
        assert(&num != &denom);
        size_type q_res_digits = num.digits - denom.digits + 1;
        if (quot.allocated < q_res_digits || &quot == &denom) {
            natural_number_base tmp_quot(q_res_digits, digit_unit);
            quotrem_number_step3(tmp_quot, num, denom, shift);
            tmp_quot.swap(quot);
        } else {
            quotrem_number_step3(quot, num, denom, shift);
        }
    }

    static void quotrem_number_step3(natural_number_base& q, natural_number_base& num, const natural_number_base& denom, unsigned shift)
    {
        LowLevel::quotrem(q.digit_array, num.digit_array, num.digits, denom.digit_array, denom.digits);
        q.set_digits_1(num.digits - denom.digits + 1);
        if (shift)
            LowLevel::rshift(num.digit_array, num.digit_array, denom.digits, shift);
        num.set_digits_n(denom.digits);  // num is now the remainder
    }

    digit_type quotrem_digit(const natural_number_base& a, digit_type denom)
    {
        digit_type remainder;
        unsigned shift = digit_bits - 1 - integer_binary_logarithm(denom);
        if (shift) {
            denom <<= shift;
            natural_number_base num(a.digits, digit_unit);
            num.digits = a.digits;
            digit_type top_digit = LowLevel::lshift(num.digit_array, a.digit_array, a.digits, shift);
            remainder = quotrem_digit_step1(top_digit, num, denom);
            remainder >>= shift;
        } else {
            remainder = quotrem_digit_step1(0, a, denom);
        }
        return remainder;
    }

    digit_type quotrem_digit_step1(digit_type top_digit, const natural_number_base& num, digit_type denom)
    {
        assert(denom & (digit_type(1) << (digit_bits - 1)));  // denominator normalized
        size_type res_digits = num.digits;
        if (!size_ok(res_digits)) {
            natural_number_base quot(res_digits, digit_unit);
            digit_type remainder = quot.quotrem_digit_step2(top_digit, num, denom);
            swap(quot);
            return remainder;
        } else
            return quotrem_digit_step2(top_digit, num, denom);
    }

    digit_type quotrem_digit_step2(digit_type top_digit, const natural_number_base& num, digit_type denom)
    {
        digit_type remainder = LowLevel::quotrem_1(digit_array, top_digit, num.digit_array, num.digits, denom);
        set_digits_1(num.digits);
        return remainder;
    }

    void bitwise_and_helper(const natural_number_base& a, const natural_number_base& b)
    {
        size_type res_digits = std::min(a.digits, b.digits);
        LowLevel::bitwise_and(digit_array, a.digit_array, b.digit_array, res_digits);
        set_digits_n(res_digits);
    }

    void bitwise_or_step2(const natural_number_base& a, const natural_number_base& b)
    {
        assert(a.digits >= b.digits);
        LowLevel::bitwise_or(digit_array, a.digit_array, b.digit_array, b.digits);
        LowLevel::copy_forward(digit_array + b.digits, a.digit_array + b.digits, a.digits - b.digits);
        digits = a.digits;
    }

    void bitwise_or_step1(const natural_number_base& a, const natural_number_base& b)
    {
        if (a.digits >= b.digits)
            bitwise_or_step2(a, b);
        else
            bitwise_or_step2(b, a);
    }

    void bitwise_xor_step2(const natural_number_base& a, const natural_number_base& b)
    {
        assert(a.digits >= b.digits);
        LowLevel::bitwise_xor(digit_array, a.digit_array, b.digit_array, b.digits);
        LowLevel::copy_forward(digit_array + b.digits, a.digit_array + b.digits, a.digits - b.digits);
        digits = a.digits;
    }

    void bitwise_xor_step1(const natural_number_base& a, const natural_number_base& b)
    {
        if (a.digits >= b.digits)
            bitwise_xor_step2(a, b);
        else
            bitwise_xor_step2(b, a);
    }

    void bitwise_and_not_helper(const natural_number_base& a, const natural_number_base& b)
    {
        if (a.digits <= b.digits) {
            LowLevel::bitwise_and_not(digit_array, a.digit_array, b.digit_array, a.digits);
        } else {
            LowLevel::bitwise_and_not(digit_array, a.digit_array, b.digit_array, b.digits);
            LowLevel::copy_forward(digit_array + b.digits, a.digit_array + b.digits, a.digits - b.digits);
        }
        set_digits_n(a.digits);
    }

    void left_shift_helper(const natural_number_base& a, unsigned long count)
    {
        assert(count != 0);
        size_type whole = count / digit_bits;
        count %= digit_bits;
        size_type new_digits = a.digits + whole;
        if (count) {
            digit_type top_digit = LowLevel::lshift(digit_array + whole, a.digit_array, a.digits, count);
            if (top_digit) {
                digit_array[a.digits + whole] = top_digit;
                ++new_digits;
            }
        } else {
            LowLevel::copy_forward(digit_array + whole, a.digit_array, a.digits);
        }
        LowLevel::fill_zero(digit_array, whole);
        digits = new_digits;
    }

    void right_shift_helper(const natural_number_base& a, unsigned long count)
    {
        assert(count != 0);
        size_type whole = count / digit_bits;
        count %= digit_bits;
        if (count) {
            LowLevel::rshift(digit_array, a.digit_array + whole, a.digits - whole, count);
        } else {
            LowLevel::copy_backward(digit_array, a.digit_array + whole, a.digits - whole);
        }
        set_digits_1(a.digits - whole);
    }

    void allocate(size_type size)
    {
        allocated = size;
        digit_array = allocator().allocate(allocated);
    }

    inline void clear()
    {
        digits = allocated = 0;
        digit_array = 0;
    }

    allocator_type& allocator() { return *this; }
    digit_type* digit_array;
    size_type digits;
    size_type allocated;
};

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_NATURAL_NUMBER_BASE_HPP
