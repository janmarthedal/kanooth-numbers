#ifndef KANOOTH_NUMBERS_BOOST_NATNUM_HPP
#define KANOOTH_NUMBERS_BOOST_NATNUM_HPP

#include <boost/multiprecision/number.hpp>
#include <boost/multiprecision/detail/integer_ops.hpp>
#include <boost/multiprecision/detail/digits.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/cstdint.hpp>
#include <limits>

#include <kanooth/numbers/integer_base.hpp>
#include <kanooth/numbers/natural_number.hpp>

namespace boost {
namespace multiprecision {
namespace backends {

template <typename B>
struct kanooth_natnum;

} // namespace backends

template <typename B>
struct number_category<backends::kanooth_natnum<B> > : public mpl::int_<number_kind_integer> {};

namespace backends {

template <typename B>
class kanooth_natnum : public kanooth::numbers::natural_number<B>
{
public:
    typedef mpl::list<long>          signed_types;
    typedef mpl::list<unsigned long> unsigned_types;
    typedef mpl::list<>              float_types;

    template <typename T, typename R>
    struct if_supported_int
        : public enable_if<mpl::or_<mpl::contains<unsigned_types, T>,
                                    mpl::contains<signed_types, T> >, R> {};

    kanooth_natnum() : base_type() {}
    //kanooth_natnum(long v) : integer_base(v) {}
    kanooth_natnum(unsigned long v) : base_type(v) {}
    kanooth_natnum(const char* s) : base_type(s) {}

private:
    typedef kanooth::numbers::natural_number<B> base_type;
};

namespace {
    template <typename T>
    typename boost::make_unsigned<T>::type to_unsigned(T v)
    {
        return static_cast<typename boost::make_unsigned<T>::type>(v);
    }
}

template <typename B>
inline bool eval_is_zero(const kanooth_natnum<B>& val)
{ return val.is_zero(); }

template <typename B>
inline void eval_add(kanooth_natnum<B>& r, const kanooth_natnum<B>& a)
{
    r.add(r, a);
}
template <typename B>
inline void eval_add(kanooth_natnum<B>& r, const kanooth_natnum<B>& a, const kanooth_natnum<B>& b)
{
    r.add(a, b);
}
template <typename B>
inline void eval_subtract(kanooth_natnum<B>& r, const kanooth_natnum<B>& a)
{
    r.subtract(r, a);
}
template <typename B>
inline void eval_subtract(kanooth_natnum<B>& r, const kanooth_natnum<B>& a, const kanooth_natnum<B>& b)
{
    r.subtract(a, b);
}
template <typename B>
inline void eval_multiply(kanooth_natnum<B>& r, const kanooth_natnum<B>& a)
{
    r.multiply(r, a);
}
template <typename B>
inline void eval_multiply(kanooth_natnum<B>& r, const kanooth_natnum<B>& a, const kanooth_natnum<B>& b)
{
    r.multiply(a, b);
}
template <typename B>
inline void eval_divide(kanooth_natnum<B>& r, const kanooth_natnum<B>& a)
{
    r.divide(r, a);
}
template <typename B>
inline void eval_divide(kanooth_natnum<B>& r, const kanooth_natnum<B>& a, const kanooth_natnum<B>& b)
{
    r.divide(a, b);
}
template <typename B>
inline void eval_modulus(kanooth_natnum<B>& r, const kanooth_natnum<B>& a)
{
    r.modulus(r, a);
}
template <typename B>
inline void eval_modulus(kanooth_natnum<B>& r, const kanooth_natnum<B>& a, const kanooth_natnum<B>& b)
{
    r.modulus(a, b);
}


template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, void>::type
eval_add(kanooth_natnum<B>& r, T a)
{
    r.add(r, to_unsigned(a));
}

template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, void>::type
eval_subtract(kanooth_natnum<B>& r, T a)
{
    r.subtract(r, to_unsigned(a));
}

template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, void>::type
eval_multiply(kanooth_natnum<B>& r, T a)
{
    r.multiply(r, to_unsigned(a));
}

template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, void>::type
eval_divide(kanooth_natnum<B>& r, T a)
{
    r.divide(r, to_unsigned(a));
}

template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, void>::type
eval_modulus(kanooth_natnum<B>& r, T a)
{
    r.modulus(r, to_unsigned(a));
}

template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, void>::type
eval_add(kanooth_natnum<B>& r, kanooth_natnum<B>& a, T b)
{
    r.add(a, to_unsigned(b));
}

template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, void>::type
eval_subtract(kanooth_natnum<B>& r, kanooth_natnum<B>& a, T b)
{
    r.subtract(a, to_unsigned(b));
}

template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, void>::type
eval_multiply(kanooth_natnum<B>& r, kanooth_natnum<B>& a, T b)
{
    r.multiply(a, to_unsigned(b));
}

template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, void>::type
eval_divide(kanooth_natnum<B>& r, kanooth_natnum<B>& a, T b)
{
    r.divide(a, to_unsigned(b));
}

template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, void>::type
eval_modulus(kanooth_natnum<B>& r, kanooth_natnum<B>& a, T b)
{
    r.modulus(a, to_unsigned(b));
}


template <typename B, typename T>
inline void eval_qr(const kanooth_natnum<B>& x, const kanooth_natnum<B>& y, kanooth_natnum<B>& q, kanooth_natnum<B>& r)
{
    kanooth_natnum<B>::quotrem(q, r, x, y);
}

template <typename B, class Integer>
inline typename enable_if<is_unsigned<Integer>, Integer>::type eval_integer_modulus(const kanooth_natnum<B>& x, Integer val)
{
    if ((sizeof(Integer) <= sizeof(long)) || (val <= (std::numeric_limits<unsigned long>::max)())) {
        return kanooth_natnum<B>::integer_modulus(x, static_cast<unsigned long>(val));
    } else {
        return default_ops::eval_integer_modulus(x, val);
    }
}

template <typename B, class Integer>
inline typename enable_if<is_signed<Integer>, Integer>::type eval_integer_modulus(const kanooth_natnum<B>& x, Integer val)
{
   typedef typename make_unsigned<Integer>::type unsigned_type;
   return eval_integer_modulus(x, static_cast<unsigned_type>(std::abs(val)));
}

template <typename B>
inline void eval_bitwise_and(kanooth_natnum<B>& result, const kanooth_natnum<B>& v)
{
   result.bitwise_and(result, v);
}

template <typename B>
inline void eval_bitwise_or(kanooth_natnum<B>& result, const kanooth_natnum<B>& v)
{
   result.bitwise_or(result, v);
}

template <typename B>
inline void eval_bitwise_xor(kanooth_natnum<B>& result, const kanooth_natnum<B>& v)
{
   result.bitwise_xor(result, v);
}

template <typename B>
inline void eval_bitwise_and(kanooth_natnum<B>& result, const kanooth_natnum<B>& u, const kanooth_natnum<B>& v)
{
   result.bitwise_and(u, v);
}

template <typename B>
inline void eval_bitwise_or(kanooth_natnum<B>& result, const kanooth_natnum<B>& u, const kanooth_natnum<B>& v)
{
   result.bitwise_or(u, v);
}

template <typename B>
inline void eval_bitwise_xor(kanooth_natnum<B>& result, const kanooth_natnum<B>& u, const kanooth_natnum<B>& v)
{
   result.bitwise_xor(u, v);
}

template <typename B>
inline void eval_left_shift(kanooth_natnum<B>& r, unsigned long v)
{
   r.left_shift(r, v);
}

template <typename B>
inline void eval_left_shift(kanooth_natnum<B>& r, const kanooth_natnum<B>& u, unsigned long v)
{
   r.left_shift(u, v);
}

template <typename B>
inline void eval_right_shift(kanooth_natnum<B>& r, unsigned long v)
{
   r.right_shift(r, v);
}

template <typename B>
inline void eval_right_shift(kanooth_natnum<B>& r, const kanooth_natnum<B>& u, unsigned long v)
{
   r.right_shift(u, v);
}

template <typename B>
inline bool eval_bit_test(const kanooth_natnum<B>& u, unsigned pos)
{
    return u.bit_test(pos);
}

template <typename B>
inline void eval_bit_set(kanooth_natnum<B>& u, unsigned pos)
{
    u.bit_set(pos);
}

template <typename B>
inline void eval_bit_unset(kanooth_natnum<B>& u, unsigned pos)
{
    u.bit_unset(pos);
}

template <typename B>
inline void eval_bit_flip(kanooth_natnum<B>& u, unsigned pos)
{
    u.bit_flip(pos);
}

template <typename B>
inline unsigned eval_lsb(const kanooth_natnum<B>& u)
{
    return u.lsb();
}

template <typename B>
inline void eval_gcd(kanooth_natnum<B>& result, const kanooth_natnum<B>& a, const kanooth_natnum<B>& b)
{
    result.gcd(a, b);
}

template <typename B>
void eval_lcm(kanooth_natnum<B>& result, const kanooth_natnum<B>& a, const kanooth_natnum<B>& b)
{
    kanooth_natnum<B> t;
    eval_gcd(t, a, b);

    if (eval_is_zero(t)) {
        result = 0ul;
    } else {
        eval_divide(result, a, t);
        eval_multiply(result, b);
    }
}

template <typename B>
inline bool eval_eq(const kanooth_natnum<B>& a, const kanooth_natnum<B>& b)
{
    return a.compare(b) == 0;
}

template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, bool>::type
eval_eq(const kanooth_natnum<B>& a, T b)
{
    return a.compare(to_unsigned(b)) == 0;
}

template <typename B>
inline bool eval_lt(const kanooth_natnum<B>& a, const kanooth_natnum<B>& b)
{
    return a.compare(b) < 0;
}

template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, bool>::type
eval_lt(const kanooth_natnum<B>& a, T b)
{
    return a.compare(b) < 0;
}

template <typename B>
inline bool eval_gt(const kanooth_natnum<B>& a, const kanooth_natnum<B>& b)
{
    return a.compare(b) > 0;
}

template <typename B, typename T>
inline typename kanooth_natnum<B>::template if_supported_int<T, bool>::type
eval_gt(const kanooth_natnum<B>& a, T b)
{
    return a.compare(b) > 0;
}

} // namespace backends
} // namespace multiprecision
} // namespace boost

namespace kanooth {
namespace numbers {

typedef boost::multiprecision::number<boost::multiprecision::backends::kanooth_natnum<best_low_level> > boost_natnum;

} // namespace numbers
} // namespace kanooth

namespace std {

template <typename B, boost::multiprecision::expression_template_option ExpressionTemplates>
class numeric_limits<boost::multiprecision::number<boost::multiprecision::backends::kanooth_natnum<B>, ExpressionTemplates> >
{
   typedef boost::multiprecision::number<boost::multiprecision::backends::kanooth_natnum<B>, ExpressionTemplates> number_type;
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

#endif // KANOOTH_NUMBERS_BOOST_NATNUM_HPP
