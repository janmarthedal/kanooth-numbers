#ifndef KANOOTH_NUMBERS_NATURAL_NUMBER_HPP
#define KANOOTH_NUMBERS_NATURAL_NUMBER_HPP

#include <memory>
#include <sstream>
#include <string>

#include <kanooth/numbers/lowlevel/generic.hpp>

namespace kanooth {
namespace numbers {
        
template <typename T = unsigned long, typename LOWLEVEL = lowlevel::generic<>, typename A = std::allocator<T> >
class natural_number {
    typedef typename A::value_type digit_type;
    typedef typename A::pointer digit_ptr;
    typedef typename A::size_type size_type;
public:
    natural_number()
        : digit_array(0), digits(0), allocated(0) {}
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
    natural_number& operator=(unsigned long v) {
        natural_number local(v);
        swap(local);
        return *this;
    }
    natural_number& operator=(const natural_number& other) {
        natural_number local(other);
        swap(local);
        return *this;
    }
    ~natural_number() {
        if (allocated) {
            allocator.deallocate(digit_array, allocated);
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
        if (a.is_zero()) {
            *this = b;
        } else if (b.is_zero()) {
            *this = a;
        } else {
            size_type min_digits = std::max(a.digits, b.digits) + 1;
            if (allocated < min_digits) {
                natural_number other(min_digits, false);
                other.add_helper(a, b);
                swap(other);
            } else {
                add_helper(a, b);
            }
        }
    }
    void subtract(const natural_number& a, const natural_number& b) {
        if (b.is_zero()) {
            *this = a;
        } else {
            size_type min_digits = a.digits;
            if (allocated < min_digits) {
                natural_number other(min_digits, false);
                other.subtract_helper(a, b);
                swap(other);
            } else {
                subtract_helper(a, b);
            }
        }
    }
    void multiply(const natural_number& a, const natural_number& b) {
        if (a.is_zero() || b.is_zero()) {
            natural_number other;
            swap(other);
        } else {
            size_type res_digits = a.digits + b.digits;
            if (allocated < res_digits || this == &a || this == &b) {
                natural_number other(res_digits, false);
                other.multiply_helper(a, b);
                swap(other);
            } else {
                multiply_helper(a, b);
            }
        }
    }
    void divide(const natural_number& a, const natural_number& b) {
        if (b.is_zero())
        {} //throw std::runtime_error("divide by zero");
        else if (a.digits < b.digits) {
            natural_number other;
            swap(other);
        } else if (&a == &b) {
            natural_number other(1u);
            swap(other);
        } else {
            natural_number r(b.digits);  // work space only
            size_type res_digits = a.digits - b.digits + 1;
            if (allocated < res_digits || this == &a || this == &b) {
                natural_number other(res_digits, false);
                quotrem_helper(other, r, a, b);
                swap(other);
            } else {
                quotrem_helper(*this, r, a, b);
            }
        }
    }
    std::string str(std::streamsize /*digits*/, std::ios_base::fmtflags f) const {
        if (is_zero())
            return "0";
        std::stringstream ss;
        ss << digit_array[0];
        return ss.str();        
    }
private:
    natural_number(size_type min_size, bool) {
        allocate(min_size);
    }
    void add_helper(const natural_number& a, const natural_number& b) {
        if (a.digits >= b.digits) {
            LOWLEVEL::add(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
            digits = a.digits;
        } else {
            LOWLEVEL::add(digit_array, b.digit_array, b.digits, a.digit_array, a.digits);
            digits = b.digits;
        }
        if (digit_array[digits])
            ++digits;
    }
    void subtract_helper(const natural_number& a, const natural_number& b) {
        LOWLEVEL::sub(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
        digits = a.digits;
        while (digits && !digit_array[digits-1])
            --digits;
    }
    void multiply_helper(const natural_number& a, const natural_number& b) {
        if (a.digits >= b.digits) {
            LOWLEVEL::mul(digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
        } else {
            LOWLEVEL::mul(digit_array, b.digit_array, b.digits, a.digit_array, a.digits);
        }
        digits = a.digits + b.digits;
        if (!digit_array[digits-1])
            --digits;
    }
    static void quotrem_helper(natural_number& q, natural_number& r, const natural_number& a, const natural_number& b) {
        LOWLEVEL::quotrem(q.digit_array, r.digit_array, a.digit_array, a.digits, b.digit_array, b.digits);
        q.digits = a.digits - b.digits;
        if (q.digit_array[q.digits])
            ++q.digits;
        r.digits = b.digits;
        while (r.digits && !r.digit_array[r.digits])
            --r.digits;
    }
    void allocate(size_type size) {
        allocated = size;
        digit_array = allocator.allocate(allocated);
    }
    
    A allocator;
    digit_ptr digit_array;
    size_type digits;
    size_type allocated;
};

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_NATURAL_NUMBER_HPP

