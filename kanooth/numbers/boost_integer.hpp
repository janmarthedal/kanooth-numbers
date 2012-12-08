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

template <typename N>
struct kanooth_integer;

} // namespace backends

template <typename N>
struct number_category<backends::kanooth_integer<N> > : public mpl::int_<number_kind_integer> {};

namespace backends {

template <typename N>
class kanooth_integer : public kanooth::numbers::integer_base<N>
{
public:
    // needed by front-end
    typedef mpl::list<long>          signed_types;
    typedef mpl::list<unsigned long> unsigned_types;
    typedef mpl::list<>              float_types;

    template <typename T, typename R>
    struct if_supported_int
        : public enable_if<mpl::or_<mpl::contains<unsigned_types, T>,
                                    mpl::contains<signed_types, T> >, R> {};

    kanooth_integer() : base_type() {}
    kanooth_integer(long v) : base_type(v) {}
    kanooth_integer(unsigned long v) : base_type(v) {}
    kanooth_integer(const char* s) : base_type(s) {}
private:
    typedef kanooth::numbers::integer_base<N> base_type;
};

template <typename N>
inline bool eval_is_zero(const kanooth_integer<N>& val)
{
    return val.is_zero();
}

template <typename N>
inline void eval_add(kanooth_integer<N>& r, const kanooth_integer<N>& a)
{
    r.add(r, a);
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, void>::type
eval_add(kanooth_integer<N>& r, T a)
{
    r.add(r, a);
}

template <typename N>
inline void eval_add(kanooth_integer<N>& r, const kanooth_integer<N>& a, const kanooth_integer<N>& b)
{
    r.add(a, b);
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, void>::type
eval_add(kanooth_integer<N>& r, const kanooth_integer<N>& a, T b)
{
    r.add(a, b);
}

template <typename N>
inline void eval_subtract(kanooth_integer<N>& r, const kanooth_integer<N>& a)
{
    r.subtract(r, a);
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, void>::type
eval_subtract(kanooth_integer<N>& r, T a)
{
    r.subtract(r, a);
}

template <typename N>
inline void eval_subtract(kanooth_integer<N>& r, const kanooth_integer<N>& a, const kanooth_integer<N>& b)
{
    r.subtract(a, b);
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, void>::type
eval_subtract(kanooth_integer<N>& r, const kanooth_integer<N>& a, T b)
{
    r.subtract(a, b);
}

template <typename N>
inline void eval_multiply(kanooth_integer<N>& r, const kanooth_integer<N>& a)
{
    r.multiply(r, a);
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, void>::type
eval_multiply(kanooth_integer<N>& r, T a)
{
    r.multiply(r, a);
}

template <typename N>
inline void eval_multiply(kanooth_integer<N>& r, const kanooth_integer<N>& a, const kanooth_integer<N>& b)
{
    r.multiply(a, b);
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, void>::type
eval_multiply(kanooth_integer<N>& r, const kanooth_integer<N>& a, T b)
{
    r.multiply(a, b);
}

template <typename N>
inline void eval_divide(kanooth_integer<N>& r, const kanooth_integer<N>& a)
{
    r.divide_truncate(r, a);
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, void>::type
eval_divide(kanooth_integer<N>& r, T a)
{
    r.divide_truncate(r, a);
}

template <typename N>
inline void eval_divide(kanooth_integer<N>& r, const kanooth_integer<N>& a, const kanooth_integer<N>& b)
{
    r.divide_truncate(a, b);
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, void>::type
eval_divide(kanooth_integer<N>& r, const kanooth_integer<N>& a, T b)
{
    r.divide_truncate(a, b);
}

template <typename N>
inline void eval_modulus(kanooth_integer<N>& r, const kanooth_integer<N>& a)
{
    r.modulus_truncate(r, a);
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, void>::type
eval_modulus(kanooth_integer<N>& r, T a)
{
    r.modulus_truncate(r, a);
}

template <typename N>
inline void eval_modulus(kanooth_integer<N>& r, const kanooth_integer<N>& a, const kanooth_integer<N>& b)
{
    r.modulus_truncate(a, b);
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, void>::type
eval_modulus(kanooth_integer<N>& r, const kanooth_integer<N>& a, T b)
{
    r.modulus_truncate(a, b);
}

template <typename N>
inline void eval_qr(const kanooth_integer<N>& x, const kanooth_integer<N>& y, kanooth_integer<N>& q, kanooth_integer<N>& r)
{
    kanooth_integer<N>::quotrem(q, r, x, y);
}

template <typename N>
inline void eval_gcd(kanooth_integer<N>& result, const kanooth_integer<N>& a, const kanooth_integer<N>& b)
{
    result.gcd(a, b);
}

template <typename N, class Integer>
inline typename enable_if<is_unsigned<Integer>, Integer>::type eval_integer_modulus(const kanooth_integer<N>& x, Integer val)
{
    if ((sizeof(Integer) <= sizeof(long)) || (val <= (std::numeric_limits<unsigned long>::max)())) {
        return kanooth_integer<N>::integer_modulus(x, static_cast<unsigned long>(val));
    } else {
        return default_ops::eval_integer_modulus(x, val);
    }
}

template <typename N, class Integer>
inline typename enable_if<is_signed<Integer>, Integer>::type eval_integer_modulus(const kanooth_integer<N>& x, Integer val)
{
   typedef typename make_unsigned<Integer>::type unsigned_type;
   return eval_integer_modulus(x, static_cast<unsigned_type>(std::abs(val)));
}

template <typename N>
inline void eval_bitwise_and(kanooth_integer<N>& result, const kanooth_integer<N>& v)
{
   result.bitwise_and(result, v);
}

template <typename N>
inline void eval_bitwise_or(kanooth_integer<N>& result, const kanooth_integer<N>& v)
{
   result.bitwise_or(result, v);
}

template <typename N>
inline void eval_bitwise_xor(kanooth_integer<N>& result, const kanooth_integer<N>& v)
{
   result.bitwise_xor(result, v);
}

template <typename N>
inline void eval_bitwise_and(kanooth_integer<N>& result, const kanooth_integer<N>& u, const kanooth_integer<N>& v)
{
   result.bitwise_and(u, v);
}

template <typename N>
inline void eval_bitwise_or(kanooth_integer<N>& result, const kanooth_integer<N>& u, const kanooth_integer<N>& v)
{
   result.bitwise_or(u, v);
}

template <typename N>
inline void eval_bitwise_xor(kanooth_integer<N>& result, const kanooth_integer<N>& u, const kanooth_integer<N>& v)
{
   result.bitwise_xor(u, v);
}

template <typename N>
inline void eval_left_shift(kanooth_integer<N>& r, unsigned long v)
{
   r.left_shift(r, v);
}

template <typename N>
inline void eval_left_shift(kanooth_integer<N>& r, kanooth_integer<N>& u, unsigned long v)
{
   r.left_shift(u, v);
}

template <typename N>
inline void eval_right_shift(kanooth_integer<N>& r, unsigned long v)
{
   r.right_shift(r, v);
}

template <typename N>
inline void eval_right_shift(kanooth_integer<N>& r, kanooth_integer<N>& u, unsigned long v)
{
   r.right_shift(u, v);
}

template <typename N>
inline bool eval_bit_test(const kanooth_integer<N>& u, unsigned pos)
{
    return u.bit_test(pos);
}

template <typename N>
inline void eval_bit_set(kanooth_integer<N>& u, unsigned pos)
{
    u.bit_set(pos);
}

template <typename N>
inline void eval_bit_unset(kanooth_integer<N>& u, unsigned pos)
{
    u.bit_unset(pos);
}

template <typename N>
inline void eval_bit_flip(kanooth_integer<N>& u, unsigned pos)
{
    u.bit_flip(pos);
}

template <typename N>
inline bool eval_eq(const kanooth_integer<N>& a, const kanooth_integer<N>& b)
{
    return a.compare(b) == 0;
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, bool>::type
eval_eq(const kanooth_integer<N>& a, T b)
{
    return a.compare(b) == 0;
}

template <typename N>
inline bool eval_lt(const kanooth_integer<N>& a, const kanooth_integer<N>& b)
{
    return a.compare(b) < 0;
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, bool>::type
eval_lt(const kanooth_integer<N>& a, T b)
{
    return a.compare(b) < 0;
}

template <typename N>
inline bool eval_gt(const kanooth_integer<N>& a, const kanooth_integer<N>& b)
{
    return a.compare(b) > 0;
}

template <typename N, typename T>
inline typename kanooth_integer<N>::template if_supported_int<T, bool>::type
eval_gt(const kanooth_integer<N>& a, T b)
{
    return a.compare(b) > 0;
}

} // namespace backends
} // namespace multiprecision
} // namespace boost

namespace kanooth {
namespace numbers {

typedef boost::multiprecision::number<boost::multiprecision::backends::kanooth_integer<natural_number<> > > boost_integer;

} // namespace numbers
} // namespace kanooth

namespace std {

template <typename N, boost::multiprecision::expression_template_option ExpressionTemplates>
class numeric_limits<boost::multiprecision::number<boost::multiprecision::backends::kanooth_integer<N>, ExpressionTemplates> >
{
   typedef boost::multiprecision::number<boost::multiprecision::backends::kanooth_integer<N>, ExpressionTemplates> number_type;
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
   BOOST_STATIC_CONSTEXPR bool is_signed = true;
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
