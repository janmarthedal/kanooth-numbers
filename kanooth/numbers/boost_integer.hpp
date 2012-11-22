#ifndef KANOOTH_NUMBERS_BOOST_INTEGER_HPP
#define KANOOTH_NUMBERS_BOOST_INTEGER_HPP

#include <boost/multiprecision/number.hpp>
#include <boost/multiprecision/detail/integer_ops.hpp>
#include <boost/multiprecision/detail/digits.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/cstdint.hpp>
#include <limits>

#include <kanooth/numbers/integer_base.hpp>
#include <kanooth/numbers/natural_number.hpp>

namespace boost {
namespace multiprecision {
namespace backends {

struct kanooth_integer;

} // namespace backends

template<>
struct number_category<backends::kanooth_integer> : public mpl::int_<number_kind_integer>{};

namespace backends {

struct kanooth_integer : public kanooth::numbers::integer_base<kanooth::numbers::natural_number<> >
{
    // needed by front-end
    typedef mpl::list<long>            signed_types;
    typedef mpl::list<unsigned long>   unsigned_types;
    typedef mpl::list<>                float_types;
    // extra
    typedef mpl::list<long, unsigned long> native_types;

    kanooth_integer() : integer_base() {}
    kanooth_integer(long v) : integer_base(v) {}
    kanooth_integer(unsigned long v) : integer_base(v) {}
    kanooth_integer(const char* s) : integer_base(s) {}
};

inline bool eval_is_zero(const kanooth_integer& val)
{ return val.is_zero(); }

inline void eval_add(kanooth_integer& r, const kanooth_integer& a)
{ r.add(r, a); }
inline void eval_add(kanooth_integer& r, unsigned long a)
{ r.add(r, a); }
inline void eval_add(kanooth_integer& r, signed long a)
{ r.add(r, a); }
inline void eval_add(kanooth_integer& r, const kanooth_integer& a, const kanooth_integer& b)
{ r.add(a, b); }
inline void eval_add(kanooth_integer& r, const kanooth_integer& a, unsigned long b)
{ r.add(a, b); }
inline void eval_add(kanooth_integer& r, const kanooth_integer& a, signed long b)
{ r.add(a, b); }

inline void eval_subtract(kanooth_integer& r, const kanooth_integer& a)
{ r.subtract(r, a); }
inline void eval_subtract(kanooth_integer& r, unsigned long a)
{ r.subtract(r, a); }
inline void eval_subtract(kanooth_integer& r, signed long a)
{ r.subtract(r, a); }
inline void eval_subtract(kanooth_integer& r, const kanooth_integer& a, const kanooth_integer& b)
{ r.subtract(a, b); }
inline void eval_subtract(kanooth_integer& r, const kanooth_integer& a, unsigned long b)
{ r.subtract(a, b); }
inline void eval_subtract(kanooth_integer& r, const kanooth_integer& a, signed long b)
{ r.subtract(a, b); }

inline void eval_multiply(kanooth_integer& r, const kanooth_integer& a)
{ r.multiply(r, a); }
inline void eval_multiply(kanooth_integer& r, unsigned long a)
{ r.multiply(r, a); }
inline void eval_multiply(kanooth_integer& r, signed long a)
{ r.multiply(r, a); }
inline void eval_multiply(kanooth_integer& r, const kanooth_integer& a, const kanooth_integer& b)
{ r.multiply(a, b); }
inline void eval_multiply(kanooth_integer& r, const kanooth_integer& a, unsigned long b)
{ r.multiply(a, b); }
inline void eval_multiply(kanooth_integer& r, const kanooth_integer& a, signed long b)
{ r.multiply(a, b); }

inline void eval_divide(kanooth_integer& r, const kanooth_integer& a)
{ r.divide_truncate(r, a); }
inline void eval_divide(kanooth_integer& r, unsigned long a)
{ r.divide_truncate(r, a); }
inline void eval_divide(kanooth_integer& r, signed long a)
{ r.divide_truncate(r, a); }
inline void eval_divide(kanooth_integer& r, const kanooth_integer& a, const kanooth_integer& b)
{ r.divide_truncate(a, b); }
inline void eval_divide(kanooth_integer& r, const kanooth_integer& a, unsigned long b)
{ r.divide_truncate(a, b); }
inline void eval_divide(kanooth_integer& r, const kanooth_integer& a, signed long b)
{ r.divide_truncate(a, b); }

inline void eval_modulus(kanooth_integer& r, const kanooth_integer& a)
{ r.modulus_truncate(r, a); }
inline void eval_modulus(kanooth_integer& r, unsigned long a)
{ r.modulus_truncate(r, a); }
inline void eval_modulus(kanooth_integer& r, signed long a)
{ r.modulus_truncate(r, a); }
inline void eval_modulus(kanooth_integer& r, const kanooth_integer& a, const kanooth_integer& b)
{ r.modulus_truncate(a, b); }
inline void eval_modulus(kanooth_integer& r, const kanooth_integer& a, unsigned long b)
{ r.modulus_truncate(a, b); }
inline void eval_modulus(kanooth_integer& r, const kanooth_integer& a, signed long b)
{ r.modulus_truncate(a, b); }

inline void eval_qr(const kanooth_integer& x, const kanooth_integer& y, kanooth_integer& q, kanooth_integer& r) {
    kanooth_integer::quotrem(q, r, x, y);
}

template <class Integer>
inline typename enable_if<is_unsigned<Integer>, Integer>::type eval_integer_modulus(const kanooth_integer& x, Integer val)
{
    if ((sizeof(Integer) <= sizeof(long)) || (val <= (std::numeric_limits<unsigned long>::max)())) {
        return kanooth_integer::integer_modulus(x, static_cast<unsigned long>(val));
    } else {
        return default_ops::eval_integer_modulus(x, val);
    }
}
template <class Integer>
inline typename enable_if<is_signed<Integer>, Integer>::type eval_integer_modulus(const kanooth_integer& x, Integer val)
{
   typedef typename make_unsigned<Integer>::type unsigned_type;
   return eval_integer_modulus(x, static_cast<unsigned_type>(std::abs(val)));
}

inline void eval_bitwise_and(kanooth_integer& result, const kanooth_integer& v)
{
   result.bitwise_and(result, v);
}

inline void eval_bitwise_or(kanooth_integer& result, const kanooth_integer& v)
{
   result.bitwise_or(result, v);
}

inline void eval_bitwise_xor(kanooth_integer& result, const kanooth_integer& v)
{
   result.bitwise_xor(result, v);
}

inline void eval_bitwise_and(kanooth_integer& result, const kanooth_integer& u, const kanooth_integer& v)
{
   result.bitwise_and(u, v);
}

inline void eval_bitwise_or(kanooth_integer& result, const kanooth_integer& u, const kanooth_integer& v)
{
   result.bitwise_or(u, v);
}

inline void eval_bitwise_xor(kanooth_integer& result, const kanooth_integer& u, const kanooth_integer& v)
{
   result.bitwise_xor(u, v);
}

inline void eval_left_shift(kanooth_integer& r, unsigned long v)
{
   r.left_shift(r, v);
}

inline void eval_left_shift(kanooth_integer& r, kanooth_integer& u, unsigned long v)
{
   r.left_shift(u, v);
}

inline void eval_right_shift(kanooth_integer& r, unsigned long v)
{
   r.right_shift(r, v);
}

inline void eval_right_shift(kanooth_integer& r, kanooth_integer& u, unsigned long v)
{
   r.right_shift(u, v);
}

inline bool eval_eq(const kanooth_integer& a, const kanooth_integer& b)
{ return a.compare(b) == 0; }

template <class T>
inline typename enable_if<mpl::contains<kanooth_integer::native_types, T>, bool>::type
eval_eq(const kanooth_integer& a, T b)
{ return a.compare(b) == 0; }

inline bool eval_lt(const kanooth_integer& a, const kanooth_integer& b)
{ return a.compare(b) < 0; }

template <class T>
inline typename enable_if<mpl::contains<kanooth_integer::native_types, T>, bool>::type
eval_lt(const kanooth_integer& a, T b)
{ return a.compare(b) < 0; }

inline bool eval_gt(const kanooth_integer& a, const kanooth_integer& b)
{ return a.compare(b) > 0; }
template <class T>
inline typename enable_if<mpl::contains<kanooth_integer::native_types, T>, bool>::type
eval_gt(const kanooth_integer& a, T b)
{ return a.compare(b) > 0; }

} // namespace backends
} // namespace multiprecision
} // namespace boost

namespace kanooth {
namespace numbers {

typedef boost::multiprecision::number<boost::multiprecision::backends::kanooth_integer> boost_integer;

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_BOOST_INTEGER_HPP
