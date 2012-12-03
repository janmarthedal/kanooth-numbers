#ifndef KANOOTH_NUMBERS_NATURAL_NUMBER_HPP
#define KANOOTH_NUMBERS_NATURAL_NUMBER_HPP

#include <memory>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cstring>  // strlen

#include <kanooth/number_bits.hpp>
#include <kanooth/numbers/lowlevel/generic.hpp>

namespace kanooth {
namespace numbers {


// TODO move to own file
template <typename T>
unsigned nlz(T v)
{
    const unsigned int digitbits = kanooth::number_bits<T>::value;
    const T mask = ((T) 1) << (digitbits - 1);
    unsigned s = 0;
    while (!(v & mask)) {
        v <<= 1;
        ++s;
    }
    return s;
}

namespace {

    template <typename T, unsigned N>
    struct pow10 {
        static const T value = T(10) * pow10<T, N-1>::value;
    };

    template <typename T>
    struct pow10<T, 0> {
        static const T value = 1;
    };

}

typedef kanooth::numbers::lowlevel::generic<unsigned long> best_low_level;

template <typename LowLevel = best_low_level, typename Allocator = std::allocator<void> >
class natural_number : private Allocator::template rebind<typename LowLevel::digit_type>::other
{

    typedef typename LowLevel::digit_type digit_type;
    typedef typename Allocator::template rebind<digit_type>::other allocator_type;
    typedef typename allocator_type::size_type size_type;
    static const unsigned digit_bits = kanooth::number_bits<digit_type>::value;
    static const unsigned digit_decimals = std::numeric_limits<digit_type>::digits10;
    static const digit_type digit_largest_pow10 = pow10<digit_type, std::numeric_limits<digit_type>::digits10>::value;

public:

    natural_number() : digit_array(0), digits(0), allocated(0) {}

    natural_number(unsigned long v)
    {
        if (v) {
            allocate(1);
            digits = 1;
            digit_array[0] = v;
        } else {
            digits = allocated = 0;
            digit_array = 0;
        }
    }

    natural_number(const natural_number& other)
    {
        if (other.digits) {
            allocate(other.digits);
            digits = other.digits;
            LowLevel::copy_forward(digit_array, other.digit_array, other.digits);
        } else {
            digits = allocated = 0;
            digit_array = 0;
        }
    }

    natural_number(const char* s) : digits(0)
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

    natural_number& operator=(unsigned long v)
    {
        natural_number(v).swap(*this);
        return *this;
    }

    // http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/
    natural_number& operator=(const natural_number& other)
    {
        if (this == &other) {
        } else if (allocated >= other.digits) {
            LowLevel::copy_forward(digit_array, other.digit_array, other.digits);
            digits = other.digits;
        } else {
            natural_number(other).swap(*this);
        }
        return *this;
    }

    natural_number& operator=(const char* s)
    {
        natural_number(s).swap(*this);
        return *this;
    }

    ~natural_number()
    {
        if (allocated) {
            allocator().deallocate(digit_array, allocated);
        }
    }

    void swap(natural_number& other)
    {
        std::swap(digit_array, other.digit_array);
        std::swap(digits, other.digits);
        std::swap(allocated, other.allocated);
    }

    bool is_zero() const
    {
        return digits == 0;
    }

    void add(const natural_number& a, const natural_number& b)
    {
        // these checks for zero are not necessary, but an optimization
        if (a.is_zero()) {
            *this = b;
        } else if (b.is_zero()) {
            *this = a;
        } else {
            size_type max_digits = std::max(a.digits, b.digits) + 1;
            if (allocated < max_digits) {
                natural_number other(max_digits, digit_unit);
                other.add_number(a, b);
                swap(other);
            } else {
                add_number(a, b);
            }
        }
    }

    void add(const natural_number& a, unsigned long b)
    {
        // these checks for zero are not necessary, but an optimization
        if (a.is_zero()) {
            *this = b;
        } else if (b == 0) {
            *this = a;
        } else {
            if (sizeof(unsigned long) <= sizeof(digit_type)) {
                size_type max_digits = a.digits + 1;
                if (allocated < max_digits) {
                    natural_number other(max_digits, digit_unit);
                    other.add_digit(a, b);
                    swap(other);
                } else {
                    add_digit(a, b);
                }
            } else {
                throw std::runtime_error("not implemented yet");
            }
        }
    }

    // result is undefined if a > b, but no crash
    void subtract(const natural_number& a, const natural_number& b)
    {
        size_type max_digits = a.digits;
        if (allocated < max_digits) {
            natural_number other(max_digits, digit_unit);
            other.subtract_number(a, b);
            swap(other);
        } else {
            subtract_number(a, b);
        }
    }

    void subtract(const natural_number& a, unsigned long b)
    {
        if (sizeof(unsigned long) <= sizeof(digit_type)) {
            size_type max_digits = a.digits;
            if (allocated < max_digits) {
                natural_number other(max_digits, digit_unit);
                other.subtract_digit(a, b);
                swap(other);
            } else {
                subtract_digit(a, b);
            }
        } else {
            throw std::runtime_error("not implemented yet");
        }
    }

    void multiply(const natural_number& a, const natural_number& b)
    {
        if (a.is_zero() || b.is_zero()) {
            *this = 0lu;
        } else {
            size_type res_digits = a.digits + b.digits;
            if (allocated < res_digits || this == &a || this == &b) {
                natural_number other(res_digits, digit_unit);
                other.multiply_number(a, b);
                swap(other);
            } else {
                multiply_number(a, b);
            }
        }
    }

    void multiply(const natural_number& a, unsigned long b)
    {
        if (b == 0) {
            *this = 0lu;
        } else if (b == 1) {
            *this = a;
        } else if (sizeof(unsigned long) <= sizeof(digit_type)) {
            size_type res_digits = a.digits + 1;
            if (allocated < res_digits) {
                natural_number other(res_digits, digit_unit);
                other.multiply_digit(a, b);
                swap(other);
            } else {
                multiply_digit(a, b);
            }
        } else {
            throw std::runtime_error("not implemented yet");
        }
    }

    void divide(const natural_number& a, const natural_number& b)
    {
        natural_number r(a.digits + 1);
        quotrem(*this, r, a, b);
    }

    inline void divide(const natural_number& a, unsigned long b)
    {
        quotrem(a, b);
    }

    void modulus(const natural_number& a, const natural_number& b)
    {
        natural_number q(a.digits - b.digits + 1);
        quotrem(q, *this, a, b);
    }

    void modulus(const natural_number& a, unsigned long b)
    {
        *this = integer_modulus(a, b);
    }

    static unsigned long integer_modulus(const natural_number& a, unsigned long b)
    {
        natural_number q(a.digits, digit_unit);
        return q.quotrem(a, b);
    }
    
    static void quotrem(natural_number& q, natural_number& r, const natural_number& a, const natural_number& b)
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
        } else {
            unsigned shift = nlz(b.digit_array[b.digits-1]);
            size_type r_res_digits = a.digits + 1;  // note: auxiliary digit
            if (r.allocated >= r_res_digits && (&r != &b || shift != 0)) {
                quotrem_number_step1(q, r, a, b, shift);
            } else {
                natural_number rem(r_res_digits, digit_unit);
                quotrem_number_step1(q, rem, a, b, shift);
                rem.swap(r);
            }
        }    
    }

    unsigned long quotrem(const natural_number& a, unsigned long b)
    {
        if (!b)
            throw std::overflow_error("division by zero");
        else if (a.is_zero()) {
            *this = 0lu;
            return b;
        } else if (sizeof(unsigned long) <= sizeof(digit_type)) {
            return quotrem_digit(a, b);
        } else {
            throw std::runtime_error("not implemented yet");
        }
    }
    
    void bitwise_and(const natural_number& a, const natural_number& b)
    {
        size_type res_digits = std::min(a.digits, b.digits);
        if (allocated < res_digits) {
            natural_number tmp(res_digits, digit_unit);
            tmp.bitwise_and_helper(a, b);
            swap(tmp);
        } else {
            bitwise_and_helper(a, b);
        }
    }

    void bitwise_or(const natural_number& a, const natural_number& b)
    {
        size_type res_digits = std::max(a.digits, b.digits);
        if (allocated < res_digits) {
            natural_number tmp(res_digits, digit_unit);
            tmp.bitwise_or_step1(a, b);
            swap(tmp);
        } else {
            bitwise_or_step1(a, b);
        }
    }

    void bitwise_xor(const natural_number& a, const natural_number& b)
    {
        size_type res_digits = std::max(a.digits, b.digits);
        if (allocated < res_digits) {
            natural_number tmp(res_digits, digit_unit);
            tmp.bitwise_xor_step1(a, b);
            swap(tmp);
        } else {
            bitwise_xor_step1(a, b);
        }
    }

    void bitwise_and_not(const natural_number& a, const natural_number& b)
    {
        size_type res_digits = a.digits;
        if (allocated < res_digits) {
            natural_number tmp(res_digits, digit_unit);
            tmp.bitwise_and_not_helper(a, b);
            swap(tmp);
        } else {
            bitwise_and_not_helper(a, b);
        }
    }

    void left_shift(const natural_number& a, unsigned long count)
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
                natural_number tmp(res_digits, digit_unit);
                tmp.left_shift_helper(a, count);
                swap(tmp);
            } else {
                left_shift_helper(a, count);
            }
        }
    }

    void right_shift(const natural_number& a, unsigned long count)
    {
        if (a.is_zero() || count >= a.digits*digit_bits)
            *this = 0ul;
        else if (!count)
            *this = a;
        else {
            size_type res_digits = a.digits - count/digit_bits;
            if (allocated < res_digits) {
                natural_number tmp(res_digits, digit_unit);
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
            natural_number tmp(index+1, digit_unit);
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

    int compare(const natural_number& a) const
    {
        if (digits < a.digits) return -1;
        if (digits > a.digits) return 1;
        return LowLevel::comp(digit_array, a.digit_array, digits);
    }

    int compare(unsigned long u) const
    {
        if (sizeof(unsigned long) <= sizeof(digit_type)) {
            if (is_zero())
                return u == 0 ? 0 : -1;
            if (digits > 1)
                return 1;
            return digit_array[0] < u ? -1 : digit_array[0] > u ? 1 : 0;
        } else {
            throw std::runtime_error("not implemented yet");            
        }
    }

    std::string str(std::streamsize /*digits*/, std::ios_base::fmtflags f) const
    {
        if (is_zero())
            return "0";
        unsigned max_decimals = (digits * digit_bits) / 3 + 1;
        char chars[max_decimals];
        char *end = chars + max_decimals;
        char *begin = end;
        natural_number n = *this;
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

private:

    enum size_request
    {
        digit_unit
    };
    
    natural_number(size_type min_size, size_request) : digits(0)
    {
        assert(min_size >= 1);
        allocate(min_size);
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

    void add_number(const natural_number& a, const natural_number& b)
    {
        if (a.digits >= b.digits) {
            digit_type carry = LowLevel::add(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
            set_digits_carry(a.digits, carry);
        } else {
            digit_type carry = LowLevel::add(digit_array, b.digit_array, b.digits, a.digit_array, a.digits);
            set_digits_carry(b.digits, carry);
        }
    }
    
    void add_digit(const natural_number& a, digit_type b)
    {
        digit_type carry = LowLevel::add_1(digit_array, a.digit_array, a.digits, b);
        set_digits_carry(a.digits, carry);
    }
    
    void subtract_number(const natural_number& a, const natural_number& b)
    {
        LowLevel::sub(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
        set_digits_n(a.digits);
    }

    void subtract_digit(const natural_number& a, digit_type b)
    {
        LowLevel::sub_1(digit_array, a.digit_array, a.digits, b);
        set_digits_n(a.digits);
    }
    
    void multiply_number(const natural_number& a, const natural_number& b)
    {
        if (a.digits >= b.digits) {
            LowLevel::mul(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
        } else {
            LowLevel::mul(digit_array, b.digit_array, b.digits, a.digit_array, a.digits);
        }
        set_digits_1(a.digits + b.digits);
    }
    
    void multiply_digit(const natural_number& a, digit_type b)
    {
        digit_type carry = LowLevel::mul_1(digit_array, a.digit_array, a.digits, b);
        set_digits_carry(a.digits, carry);
    }
    
    static void quotrem_number_step1(natural_number& q, natural_number& rem, const natural_number& a, const natural_number& b, unsigned shift)
    {
        assert(rem.allocated >= a.digits + 1);
        assert(&rem != &b || shift != 0);  // &rem == &b is ok when shift != 0 since a temporary instead of b will be used as denominator
        rem.digits = a.digits;
        if (shift) {
            natural_number denom(b.digits, digit_unit);
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

    static void quotrem_number_step2(natural_number& quot, natural_number& num, const natural_number& denom, unsigned shift)
    {
        assert(denom.digit_array[denom.digits-1] & (digit_type(1) << (digit_bits - 1)));  // denominator normalized
        assert(&num != &denom);
        size_type q_res_digits = num.digits - denom.digits + 1;
        if (quot.allocated < q_res_digits || &quot == &denom) {
            natural_number tmp_quot(q_res_digits, digit_unit);
            quotrem_number_step3(tmp_quot, num, denom, shift);
            tmp_quot.swap(quot);
        } else {
            quotrem_number_step3(quot, num, denom, shift);
        }
    }

    static void quotrem_number_step3(natural_number& q, natural_number& num, const natural_number& denom, unsigned shift)
    {
        LowLevel::quotrem(q.digit_array, num.digit_array, num.digits, denom.digit_array, denom.digits);
        q.set_digits_1(num.digits - denom.digits + 1);
        if (shift)
            LowLevel::rshift(num.digit_array, num.digit_array, denom.digits, shift);
        num.set_digits_n(denom.digits);  // num is now the remainder
    }
    
    digit_type quotrem_digit(const natural_number& a, digit_type b)
    {
        digit_type denom = b, remainder;
        unsigned shift = nlz(denom);
        if (shift) {
            denom <<= shift;
            natural_number num(a.digits, digit_unit);
            num.digits = a.digits;
            digit_type top_digit = LowLevel::lshift(num.digit_array, a.digit_array, a.digits, shift);
            remainder = quotrem_digit_step1(top_digit, num, denom);
            remainder >>= shift;
        } else {
            remainder = quotrem_digit_step1(0, a, denom);
        }
        return remainder;
    }

    digit_type quotrem_digit_step1(digit_type top_digit, const natural_number& num, digit_type denom)
    {
        assert(denom & (digit_type(1) << (digit_bits - 1)));  // denominator normalized
        size_type res_digits = num.digits;
        if (allocated < res_digits) {
            natural_number quot(res_digits, digit_unit);
            digit_type remainder = quot.quotrem_digit_step2(top_digit, num, denom);
            swap(quot);
            return remainder;
        } else
            return quotrem_digit_step2(top_digit, num, denom);
    }

    digit_type quotrem_digit_step2(digit_type top_digit, const natural_number& num, digit_type denom)
    {
        digit_type remainder = LowLevel::quotrem_1(digit_array, top_digit, num.digit_array, num.digits, denom);
        set_digits_1(num.digits);
        return remainder;
    }
    
    void bitwise_and_helper(const natural_number& a, const natural_number& b)
    {
        size_type res_digits = std::min(a.digits, b.digits);        
        LowLevel::bitwise_and(digit_array, a.digit_array, b.digit_array, res_digits);
        set_digits_n(res_digits);
    }
    
    void bitwise_or_step2(const natural_number& a, const natural_number& b)
    {
        assert(a.digits >= b.digits);
        LowLevel::bitwise_or(digit_array, a.digit_array, b.digit_array, b.digits);
        LowLevel::copy_forward(digit_array + b.digits, a.digit_array + b.digits, a.digits - b.digits);
        digits = a.digits;
    }
    
    void bitwise_or_step1(const natural_number& a, const natural_number& b)
    {
        if (a.digits >= b.digits)
            bitwise_or_step2(a, b);
        else
            bitwise_or_step2(b, a);
    }
    
    void bitwise_xor_step2(const natural_number& a, const natural_number& b)
    {
        assert(a.digits >= b.digits);
        LowLevel::bitwise_xor(digit_array, a.digit_array, b.digit_array, b.digits);
        LowLevel::copy_forward(digit_array + b.digits, a.digit_array + b.digits, a.digits - b.digits);
        digits = a.digits;
    }
    
    void bitwise_xor_step1(const natural_number& a, const natural_number& b)
    {
        if (a.digits >= b.digits)
            bitwise_xor_step2(a, b);
        else
            bitwise_xor_step2(b, a);
    }
    
    void bitwise_and_not_helper(const natural_number& a, const natural_number& b)
    {
        if (a.digits <= b.digits) {
            LowLevel::bitwise_and_not(digit_array, a.digit_array, b.digit_array, a.digits);
        } else {
            LowLevel::bitwise_and_not(digit_array, a.digit_array, b.digit_array, b.digits);
            LowLevel::copy_forward(digit_array + b.digits, a.digit_array + b.digits, a.digits - b.digits);
        }
        set_digits_n(a.digits);
    }
    
    void left_shift_helper(const natural_number& a, unsigned long count)
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
    
    void right_shift_helper(const natural_number& a, unsigned long count)
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
    
    allocator_type& allocator() { return *this; }
    digit_type* digit_array;
    size_type digits;
    size_type allocated;
};

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_NATURAL_NUMBER_HPP

