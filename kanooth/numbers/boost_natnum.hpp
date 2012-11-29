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

struct kanooth_natnum;

} // namespace backends

template<>
struct number_category<backends::kanooth_natnum> : public mpl::int_<number_kind_integer>{};

namespace backends {

struct kanooth_natnum : public kanooth::numbers::natural_number<>
{
    // needed by front-end
    typedef mpl::list<long>            signed_types;
    typedef mpl::list<unsigned long>   unsigned_types;
    typedef mpl::list<>                float_types;
    // extra
    typedef mpl::list<unsigned long, long> native_types;

    kanooth_natnum() : natural_number() {}
    //kanooth_natnum(long v) : integer_base(v) {}
    kanooth_natnum(unsigned long v) : natural_number(v) {}
    kanooth_natnum(const char* s) : natural_number(s) {}
};

inline bool eval_is_zero(const kanooth_natnum& val)
{ return val.is_zero(); }

inline void eval_add(kanooth_natnum& r, const kanooth_natnum& a)
{ r.add(r, a); }
inline void eval_add(kanooth_natnum& r, unsigned long a)
{ r.add(r, a); }
//inline void eval_add(kanooth_natnum& r, signed long a)
//{ r.add(r, a); }
inline void eval_add(kanooth_natnum& r, const kanooth_natnum& a, const kanooth_natnum& b)
{ r.add(a, b); }
inline void eval_add(kanooth_natnum& r, const kanooth_natnum& a, unsigned long b)
{ r.add(a, b); }
//inline void eval_add(kanooth_natnum& r, const kanooth_natnum& a, signed long b)
//{ r.add(a, b); }

inline void eval_subtract(kanooth_natnum& r, const kanooth_natnum& a)
{ r.subtract(r, a); }
inline void eval_subtract(kanooth_natnum& r, unsigned long a)
{ r.subtract(r, a); }
//inline void eval_subtract(kanooth_natnum& r, signed long a)
//{ r.subtract(r, a); }
inline void eval_subtract(kanooth_natnum& r, const kanooth_natnum& a, const kanooth_natnum& b)
{ r.subtract(a, b); }
inline void eval_subtract(kanooth_natnum& r, const kanooth_natnum& a, unsigned long b)
{ r.subtract(a, b); }
//inline void eval_subtract(kanooth_natnum& r, const kanooth_natnum& a, signed long b)
//{ r.subtract(a, b); }

inline void eval_multiply(kanooth_natnum& r, const kanooth_natnum& a)
{ r.multiply(r, a); }
inline void eval_multiply(kanooth_natnum& r, unsigned long a)
{ r.multiply(r, a); }
//inline void eval_multiply(kanooth_natnum& r, signed long a)
//{ r.multiply(r, a); }
inline void eval_multiply(kanooth_natnum& r, const kanooth_natnum& a, const kanooth_natnum& b)
{ r.multiply(a, b); }
inline void eval_multiply(kanooth_natnum& r, const kanooth_natnum& a, unsigned long b)
{ r.multiply(a, b); }
//inline void eval_multiply(kanooth_natnum& r, const kanooth_natnum& a, signed long b)
//{ r.multiply(a, b); }

inline void eval_divide(kanooth_natnum& r, const kanooth_natnum& a)
{ r.divide(r, a); }
inline void eval_divide(kanooth_natnum& r, unsigned long a)
{ r.divide(r, a); }
//inline void eval_divide(kanooth_natnum& r, signed long a)
//{ r.divide(r, a); }
inline void eval_divide(kanooth_natnum& r, const kanooth_natnum& a, const kanooth_natnum& b)
{ r.divide(a, b); }
inline void eval_divide(kanooth_natnum& r, const kanooth_natnum& a, unsigned long b)
{ r.divide(a, b); }
//inline void eval_divide(kanooth_natnum& r, const kanooth_natnum& a, signed long b)
//{ r.divide(a, b); }

inline void eval_modulus(kanooth_natnum& r, const kanooth_natnum& a)
{ r.modulus(r, a); }
inline void eval_modulus(kanooth_natnum& r, unsigned long a)
{ r.modulus(r, a); }
//inline void eval_modulus(kanooth_natnum& r, signed long a)
//{ r.modulus(r, a); }
inline void eval_modulus(kanooth_natnum& r, const kanooth_natnum& a, const kanooth_natnum& b)
{ r.modulus(a, b); }
inline void eval_modulus(kanooth_natnum& r, const kanooth_natnum& a, unsigned long b)
{ r.modulus(a, b); }
//inline void eval_modulus(kanooth_natnum& r, const kanooth_natnum& a, signed long b)
//{ r.modulus(a, b); }

inline void eval_qr(const kanooth_natnum& x, const kanooth_natnum& y, kanooth_natnum& q, kanooth_natnum& r) {
    kanooth_natnum::quotrem(q, r, x, y);
}

template <class Integer>
inline typename enable_if<is_unsigned<Integer>, Integer>::type eval_integer_modulus(const kanooth_natnum& x, Integer val)
{
    if ((sizeof(Integer) <= sizeof(long)) || (val <= (std::numeric_limits<unsigned long>::max)())) {
        return kanooth_natnum::modulus(x, static_cast<unsigned long>(val));
    } else {
        return default_ops::eval_integer_modulus(x, val);
    }
}
template <class Integer>
inline typename enable_if<is_signed<Integer>, Integer>::type eval_integer_modulus(const kanooth_natnum& x, Integer val)
{
   typedef typename make_unsigned<Integer>::type unsigned_type;
   return eval_integer_modulus(x, static_cast<unsigned_type>(std::abs(val)));
}

inline void eval_bitwise_and(kanooth_natnum& result, const kanooth_natnum& v)
{
   result.bitwise_and(result, v);
}

inline void eval_bitwise_or(kanooth_natnum& result, const kanooth_natnum& v)
{
   result.bitwise_or(result, v);
}

inline void eval_bitwise_xor(kanooth_natnum& result, const kanooth_natnum& v)
{
   result.bitwise_xor(result, v);
}

inline void eval_bitwise_and(kanooth_natnum& result, const kanooth_natnum& u, const kanooth_natnum& v)
{
   result.bitwise_and(u, v);
}

inline void eval_bitwise_or(kanooth_natnum& result, const kanooth_natnum& u, const kanooth_natnum& v)
{
   result.bitwise_or(u, v);
}

inline void eval_bitwise_xor(kanooth_natnum& result, const kanooth_natnum& u, const kanooth_natnum& v)
{
   result.bitwise_xor(u, v);
}

inline void eval_left_shift(kanooth_natnum& r, unsigned long v)
{
   r.left_shift(r, v);
}

inline void eval_left_shift(kanooth_natnum& r, kanooth_natnum& u, unsigned long v)
{
   r.left_shift(u, v);
}

inline void eval_right_shift(kanooth_natnum& r, unsigned long v)
{
   r.right_shift(r, v);
}

inline void eval_right_shift(kanooth_natnum& r, kanooth_natnum& u, unsigned long v)
{
   r.right_shift(u, v);
}

inline bool eval_bit_test(const kanooth_natnum& u, unsigned pos)
{
    return u.bit_test(pos);
}

inline void eval_bit_set(kanooth_natnum& u, unsigned pos)
{
    u.bit_set(pos);
}

inline void eval_bit_unset(kanooth_natnum& u, unsigned pos)
{
    u.bit_unset(pos);
}

inline void eval_bit_flip(kanooth_natnum& u, unsigned pos)
{
    u.bit_flip(pos);
}

inline bool eval_eq(const kanooth_natnum& a, const kanooth_natnum& b)
{ return a.compare(b) == 0; }

template <class T>
inline typename enable_if<mpl::contains<kanooth_natnum::native_types, T>, bool>::type
eval_eq(const kanooth_natnum& a, T b)
{ return a.compare(b) == 0; }

inline bool eval_lt(const kanooth_natnum& a, const kanooth_natnum& b)
{ return a.compare(b) < 0; }

template <class T>
inline typename enable_if<mpl::contains<kanooth_natnum::native_types, T>, bool>::type
eval_lt(const kanooth_natnum& a, T b)
{ return a.compare(b) < 0; }

inline bool eval_gt(const kanooth_natnum& a, const kanooth_natnum& b)
{ return a.compare(b) > 0; }
template <class T>
inline typename enable_if<mpl::contains<kanooth_natnum::native_types, T>, bool>::type
eval_gt(const kanooth_natnum& a, T b)
{ return a.compare(b) > 0; }

} // namespace backends
} // namespace multiprecision
} // namespace boost

namespace kanooth {
namespace numbers {

typedef boost::multiprecision::number<boost::multiprecision::backends::kanooth_natnum> boost_natnum;

} // namespace numbers
} // namespace kanooth

namespace std {

template<boost::multiprecision::expression_template_option ExpressionTemplates>
class numeric_limits<boost::multiprecision::number<boost::multiprecision::backends::kanooth_natnum, ExpressionTemplates> >
{
   typedef boost::multiprecision::number<boost::multiprecision::backends::kanooth_natnum, ExpressionTemplates> number_type;
public:
   BOOST_STATIC_CONSTEXPR bool is_specialized = true;
   //
   // Largest and smallest numbers are bounded only by available memory, set
   // to zero:
   //
   static number_type (min)() BOOST_NOEXCEPT
   {
      return number_type();
   }
   static number_type (max)() BOOST_NOEXCEPT
   {
      return number_type();
   }
   static number_type lowest() BOOST_NOEXCEPT { return (min)(); }
   BOOST_STATIC_CONSTEXPR int digits = INT_MAX;
   BOOST_STATIC_CONSTEXPR int digits10 = (INT_MAX / 1000) * 301L;
   BOOST_STATIC_CONSTEXPR int max_digits10 = digits10 + 2;
   BOOST_STATIC_CONSTEXPR bool is_signed = false;
   BOOST_STATIC_CONSTEXPR bool is_integer = true;
   BOOST_STATIC_CONSTEXPR bool is_exact = true;
   BOOST_STATIC_CONSTEXPR int radix = 2;
   static number_type epsilon() BOOST_NOEXCEPT { return number_type(); }
   static number_type round_error() BOOST_NOEXCEPT { return number_type(); }
   BOOST_STATIC_CONSTEXPR int min_exponent = 0;
   BOOST_STATIC_CONSTEXPR int min_exponent10 = 0;
   BOOST_STATIC_CONSTEXPR int max_exponent = 0;
   BOOST_STATIC_CONSTEXPR int max_exponent10 = 0;
   BOOST_STATIC_CONSTEXPR bool has_infinity = false;
   BOOST_STATIC_CONSTEXPR bool has_quiet_NaN = false;
   BOOST_STATIC_CONSTEXPR bool has_signaling_NaN = false;
   BOOST_STATIC_CONSTEXPR float_denorm_style has_denorm = denorm_absent;
   BOOST_STATIC_CONSTEXPR bool has_denorm_loss = false;
   static number_type infinity() BOOST_NOEXCEPT { return number_type(); }
   static number_type quiet_NaN() BOOST_NOEXCEPT { return number_type(); }
   static number_type signaling_NaN() BOOST_NOEXCEPT { return number_type(); }
   static number_type denorm_min() BOOST_NOEXCEPT { return number_type(); }
   BOOST_STATIC_CONSTEXPR bool is_iec559 = false;
   BOOST_STATIC_CONSTEXPR bool is_bounded = false;
   BOOST_STATIC_CONSTEXPR bool is_modulo = false;
   BOOST_STATIC_CONSTEXPR bool traps = false;
   BOOST_STATIC_CONSTEXPR bool tinyness_before = false;
   BOOST_STATIC_CONSTEXPR float_round_style round_style = round_toward_zero;
};

} // namespace std

#endif // KANOOTH_NUMBERS_BOOST_INTEGER_HPP
