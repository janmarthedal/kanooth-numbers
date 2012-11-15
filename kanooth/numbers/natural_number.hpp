#ifndef KANOOTH_NUMBERS_NATURAL_NUMBER_HPP
#define KANOOTH_NUMBERS_NATURAL_NUMBER_HPP

#include <memory>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cassert>

#include <kanooth/number_bits.hpp>
#include <kanooth/numbers/lowlevel/generic.hpp>

namespace kanooth {
namespace numbers {
        
template <typename T = unsigned long, typename Allocator = std::allocator<T>, typename LOWLEVEL = lowlevel::generic<Allocator> >
class natural_number : private Allocator::template rebind<T>::other {

    typedef typename Allocator::template rebind<T>::other allocator_type;
    typedef typename allocator_type::value_type digit_type;
    typedef typename allocator_type::pointer digit_ptr;
    typedef typename allocator_type::size_type size_type;

public:

    natural_number() : digit_array(0), digits(0), allocated(0) {}

    natural_number(unsigned long v) {
        if (v) {
            allocate(1);
            digits = 1;
            digit_array[0] = v;
        } else {
            digits = allocated = 0;
            digit_array = 0;
        }
    }

    natural_number(const natural_number& other) {
        if (this == &other) return;
        if (other.digits) {
            allocate(other.digits);
            digits = other.digits;
            std::copy(other.digit_array, other.digit_array + other.digits, digit_array);
        } else {
            digits = allocated = 0;
            digit_array = 0;
        }
    }

    natural_number(const char* s) : digits(0), allocated(0), digit_array(0) {
        while (*s) {
            multiply(*this, 10u);
            add(*this, static_cast<unsigned long>(*s - '0'));
            ++s;
        }        
    }

    natural_number& operator=(unsigned long v) {
        natural_number(v).swap(*this);
        return *this;
    }

    // Consider http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/ ?
    natural_number& operator=(const natural_number& other) {
        natural_number(other).swap(*this);
        return *this;
    }

    natural_number& operator=(const char* s) {
        natural_number(s).swap(*this);
        return *this;
    }

    ~natural_number() {
        if (allocated) {
            allocator().deallocate(digit_array, allocated);
        }
    }

    void swap(natural_number& other) {
        std::swap(digit_array, other.digit_array);
        std::swap(digits, other.digits);
        std::swap(allocated, other.allocated);
    }

    bool is_zero() const {
        return digits == 0;
    }

    void add(const natural_number& a, const natural_number& b) {
        size_type max_digits = std::max(a.digits, b.digits) + 1;
        if (allocated < max_digits) {
            natural_number other(max_digits, digit_unit);
            other.add_helper(a, b);
            swap(other);
        } else {
            add_helper(a, b);
        }
    }

    void add(const natural_number& a, unsigned long b) {
        if (sizeof(unsigned long) <= sizeof(digit_type)) {
            size_type max_digits = a.digits + 1;
            if (allocated < max_digits) {
                natural_number other(max_digits, digit_unit);
                other.add_helper(a, b);
                swap(other);
            } else {
                add_helper(a, b);
            }
        } else {
            throw std::runtime_error("not implemented yet");
        }
    }

    void subtract(const natural_number& a, const natural_number& b) {
        size_type max_digits = a.digits;
        if (allocated < max_digits) {
            natural_number other(max_digits, digit_unit);
            other.subtract_helper(a, b);
            swap(other);
        } else {
            subtract_helper(a, b);
        }
    }

    void multiply(const natural_number& a, const natural_number& b) {
        if (a.is_zero() || b.is_zero()) {
            *this = 0lu;
        } else {
            size_type res_digits = a.digits + b.digits;
            if (allocated < res_digits || this == &a || this == &b) {
                natural_number other(res_digits, digit_unit);
                other.multiply_helper(a, b);
                swap(other);
            } else {
                multiply_helper(a, b);
            }
        }
    }

    void multiply(const natural_number& a, unsigned long b) {
        if (sizeof(unsigned long) <= sizeof(digit_type)) {
            size_type res_digits = a.digits + 1;
            if (allocated < res_digits) {
                natural_number other(res_digits, digit_unit);
                other.multiply_helper(a, b);
                swap(other);
            } else {
                multiply_helper(a, b);
            }
        } else {
            throw std::runtime_error("not implemented yet");
        }
    }

    void divide(const natural_number& a, const natural_number& b) {
        if (b.is_zero())
            throw std::overflow_error("division by zero");
        else if (a.digits < b.digits) {
            *this = 0lu;
        } else if (&a == &b) {
            *this = 1lu;
        } else {
            natural_number r(b.digits, digit_unit);  // work space only
            size_type res_digits = a.digits - b.digits + 1;
            if (allocated < res_digits || this == &a || this == &b) {
                natural_number other(res_digits, digit_unit);
                quotrem_helper(other, r, a, b);
                swap(other);
            } else {
                quotrem_helper(*this, r, a, b);
            }
        }
    }

    inline void divide(const natural_number& a, unsigned long b) {
        quotrem(a, b);
    }

    void modulus(const natural_number& a, const natural_number& b) {
        if (b.is_zero())
            throw std::overflow_error("division by zero");
        else if (a.digits < b.digits) {
            *this = a;
        } else if (&a == &b) {
            *this = 0lu;
        } else {
            natural_number q(a.digits - b.digits + 1, digit_unit);  // work space only
            size_type res_digits = b.digits;
            if (allocated < res_digits || this == &a || this == &b) {
                natural_number other(res_digits, digit_unit);
                quotrem_helper(q, other, a, b);
                swap(other);
            } else {
                quotrem_helper(q, *this, a, b);
            }
        }        
    }

    unsigned long modulus(unsigned long b) const {
        natural_number q(digits, digit_unit);
        return q.quotrem(*this, b);
    }
    
    static void quotrem(natural_number& q, natural_number& r, const natural_number& a, const natural_number& b) {
        if (&q == &r)
            throw std::invalid_argument("q must be different from r");
        if (b.is_zero())
            throw std::overflow_error("division by zero");
        else if (a.digits < b.digits) {
            q = 0lu;
            r = a;
        } else if (&a == &b) {
            q = 1lu;
            r = 0lu;
        } else {
            size_type q_res_digits = a.digits - b.digits + 1;
            size_type r_res_digits = b.digits;
            bool temp_q_needed = q.allocated < q_res_digits || &q == &a || &q == &b;
            bool temp_r_needed = r.allocated < r_res_digits || &r == &a || &r == &b;
            if (temp_q_needed) {
                natural_number temp_q(q_res_digits, digit_unit);
                if (temp_r_needed) {
                    natural_number temp_r(r_res_digits, digit_unit);
                    quotrem_helper(temp_q, temp_r, a, b);
                    r.swap(temp_r);
                } else {
                    quotrem_helper(temp_q, r, a, b);
                }
                q.swap(temp_q);
            } else if (temp_r_needed) {
                natural_number temp_r(r_res_digits, digit_unit);
                quotrem_helper(q, temp_r, a, b);
                r.swap(temp_r);                
            } else {
                quotrem_helper(q, r, a, b);
            }
        }    
    }
    
    unsigned long quotrem(const natural_number& a, unsigned long b) {
        if (!b)
            throw std::overflow_error("division by zero");
        else if (a.is_zero()) {
            natural_number().swap(*this);
            return b;
        } else if (sizeof(unsigned long) <= sizeof(digit_type)) {
            size_type res_digits = a.digits;
            if (allocated < res_digits) {
                natural_number other(res_digits, digit_unit);
                digit_type r = other.quotrem_helper(a, static_cast<digit_type>(b));
                swap(other);
                return static_cast<unsigned long>(r);
            } else
                return quotrem_helper(a, b);
        } else {
            throw std::runtime_error("not implemented yet");
        }
    }
    
    int compare(const natural_number& a) const {
        if (digits < a.digits) return -1;
        if (digits > a.digits) return 1;
        return LOWLEVEL::comp(digit_array, a.digit_array, digits);
    }

    std::string str(std::streamsize /*digits*/, std::ios_base::fmtflags f) const {
        if (is_zero())
            return "0";
        unsigned max_decimals = (digits * kanooth::number_bits<digit_type>::value) / 3 + 1;
        char chars[max_decimals];
        char *end = chars + max_decimals;
        char *begin = end;
        natural_number n(*this);
        while (!n.is_zero()) {
            unsigned long d = n.quotrem(n, 10u);
            *--begin = d + '0';
        }
        return std::string(begin, end);
    }

private:

    enum size_request {
        digit_unit
    };
    
    natural_number(size_type min_size, size_request) : digits(0) {
        assert(min_size >= 1);
        allocate(min_size);
    }

    inline void set_digits_1(size_type max_digits) {
        assert(max_digits >= 1);
        digits = max_digits;
        if (!digit_array[digits-1])
            --digits;
    }

    inline void set_digits_n(size_type max_digits) {
        digits = max_digits;
        while (digits && !digit_array[digits-1])
            --digits;
    }

    inline void set_digits_carry(size_type base_digits, digit_type carry) {
        digits = base_digits;
        if (carry)
            digit_array[digits++] = carry;
    }

    void add_helper(const natural_number& a, const natural_number& b) {
        if (a.digits >= b.digits) {
            digit_type carry = LOWLEVEL::add(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
            set_digits_carry(a.digits, carry);
        } else {
            digit_type carry = LOWLEVEL::add(digit_array, b.digit_array, b.digits, a.digit_array, a.digits);
            set_digits_carry(b.digits, carry);
        }
    }
    
    void add_helper(const natural_number& a, digit_type b) {
        digit_type carry = LOWLEVEL::add_1(digit_array, a.digit_array, a.digits, b);
        set_digits_carry(a.digits, carry);
    }
    
    void subtract_helper(const natural_number& a, const natural_number& b) {
        LOWLEVEL::sub(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
        set_digits_n(a.digits);
    }
    
    void multiply_helper(const natural_number& a, const natural_number& b) {
        if (a.digits >= b.digits) {
            LOWLEVEL::mul(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
        } else {
            LOWLEVEL::mul(digit_array, b.digit_array, b.digits, a.digit_array, a.digits);
        }
        set_digits_1(a.digits + b.digits);
    }
    
    void multiply_helper(const natural_number& a, digit_type b) {
        digit_type carry = LOWLEVEL::mul_1(digit_array, a.digit_array, a.digits, b);
        set_digits_carry(a.digits, carry);
    }
    
    static void quotrem_helper(natural_number& q, natural_number& r, const natural_number& a, const natural_number& b) {
        LOWLEVEL::quotrem(q.digit_array, r.digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
        q.set_digits_1(a.digits - b.digits + 1);
        r.set_digits_n(b.digits);
    }
    
    digit_type quotrem_helper(const natural_number& a, digit_type b) {
        T r = LOWLEVEL::quotrem_1(digit_array, a.digit_array, a.digits, b);
        set_digits_1(a.digits);
        return r;
    }
    
    void allocate(size_type size) {
        allocated = size;
        digit_array = allocator().allocate(allocated);
    }
    
    allocator_type& allocator() { return *this; }
    digit_ptr digit_array;
    size_type digits;
    size_type allocated;
};

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_NATURAL_NUMBER_HPP

