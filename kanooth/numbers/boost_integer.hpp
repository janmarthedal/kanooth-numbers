#ifndef KANOOTH_NUMBERS_BOOST_INTEGER_HPP
#define KANOOTH_NUMBERS_BOOST_INTEGER_HPP

#include <boost/multiprecision/number.hpp>
#include <boost/multiprecision/detail/integer_ops.hpp>
#include <boost/multiprecision/detail/digits.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/cstdint.hpp>
#include <limits>

#include <kanooth/numbers/integer.hpp>

namespace boost {
namespace multiprecision {
namespace backends {

struct kanooth_integer;

} // namespace backends

template<>
struct number_category<backends::kanooth_integer> : public mpl::int_<number_kind_integer>{};

namespace backends {

struct kanooth_integer
{
    //typedef mpl::list<long, long long>                     signed_types;
    //typedef mpl::list<unsigned long, unsigned long long>   unsigned_types;
    //typedef mpl::list<double, long double>                 float_types;
    typedef mpl::list<long>            signed_types;
    typedef mpl::list<unsigned long>   unsigned_types;
    typedef mpl::list<>                float_types;
    
    typedef mpl::list<long, unsigned long> native_types;

    typedef ::kanooth::numbers::integer data_type;
    kanooth_integer() : m_data() {}
    kanooth_integer(long v) : m_data(v) {}
    kanooth_integer(unsigned long v) : m_data(v) {}
    kanooth_integer(const char* s) : m_data(s) {}
    kanooth_integer(const kanooth_integer& o) : m_data(o.m_data) {}
    kanooth_integer& operator= (long v) {
        m_data = v;
        return *this;
    }
    kanooth_integer& operator= (unsigned long v) {
        m_data = v;
        return *this;
    }
    kanooth_integer& operator= (const char* s) {
        m_data = s;
        return *this;
    }
    kanooth_integer& operator= (const kanooth_integer& o) {
        m_data = o.m_data;
        return *this;
    }
    void negate() {
        m_data.negate();
    }
    int compare(const kanooth_integer& other) const {
        return m_data.compare(other.m_data);
    }
    inline void swap(kanooth_integer& o) {
        m_data.swap(o.m_data);
    }
    inline std::string str(std::streamsize /*digits*/, std::ios_base::fmtflags f) const {
        return m_data.str(0, f);
    }
    data_type& data() { 
       return m_data; 
    }
    const data_type& data() const { 
       return m_data; 
    }
protected:
    data_type m_data;
};

inline bool eval_is_zero(const kanooth_integer& val)
{ return val.data().is_zero(); }

inline void eval_add(kanooth_integer& r, const kanooth_integer& a)
{ r.data().add(r.data(), a.data()); }
inline void eval_add(kanooth_integer& r, unsigned long a)
{ r.data().add(r.data(), a); }
inline void eval_add(kanooth_integer& r, signed long a)
{ r.data().add(r.data(), a); }
inline void eval_add(kanooth_integer& r, const kanooth_integer& a, const kanooth_integer& b)
{ r.data().add(a.data(), b.data()); }
inline void eval_add(kanooth_integer& r, const kanooth_integer& a, unsigned long b)
{ r.data().add(a.data(), b); }
inline void eval_add(kanooth_integer& r, const kanooth_integer& a, signed long b)
{ r.data().add(a.data(), b); }

inline void eval_subtract(kanooth_integer& r, const kanooth_integer& a)
{ r.data().subtract(r.data(), a.data()); }
inline void eval_subtract(kanooth_integer& r, unsigned long a)
{ r.data().subtract(r.data(), a); }
inline void eval_subtract(kanooth_integer& r, signed long a)
{ r.data().subtract(r.data(), a); }
inline void eval_subtract(kanooth_integer& r, const kanooth_integer& a, const kanooth_integer& b)
{ r.data().subtract(a.data(), b.data()); }
inline void eval_subtract(kanooth_integer& r, const kanooth_integer& a, unsigned long b)
{ r.data().subtract(a.data(), b); }
inline void eval_subtract(kanooth_integer& r, const kanooth_integer& a, signed long b)
{ r.data().subtract(a.data(), b); }

inline void eval_multiply(kanooth_integer& r, const kanooth_integer& a)
{ r.data().multiply(r.data(), a.data()); }
inline void eval_multiply(kanooth_integer& r, unsigned long a)
{ r.data().multiply(r.data(), a); }
inline void eval_multiply(kanooth_integer& r, signed long a)
{ r.data().multiply(r.data(), a); }
inline void eval_multiply(kanooth_integer& r, const kanooth_integer& a, const kanooth_integer& b)
{ r.data().multiply(a.data(), b.data()); }
inline void eval_multiply(kanooth_integer& r, const kanooth_integer& a, unsigned long b)
{ r.data().multiply(a.data(), b); }
inline void eval_multiply(kanooth_integer& r, const kanooth_integer& a, signed long b)
{ r.data().multiply(a.data(), b); }

inline void eval_divide(kanooth_integer& r, const kanooth_integer& a)
{ r.data().divide_truncate(r.data(), a.data()); }
inline void eval_divide(kanooth_integer& r, unsigned long a)
{ r.data().divide_truncate(r.data(), a); }
inline void eval_divide(kanooth_integer& r, signed long a)
{ r.data().divide_truncate(r.data(), a); }
inline void eval_divide(kanooth_integer& r, const kanooth_integer& a, const kanooth_integer& b)
{ r.data().divide_truncate(a.data(), b.data()); }
inline void eval_divide(kanooth_integer& r, const kanooth_integer& a, unsigned long b)
{ r.data().divide_truncate(a.data(), b); }
inline void eval_divide(kanooth_integer& r, const kanooth_integer& a, signed long b)
{ r.data().divide_truncate(a.data(), b); }

inline void eval_modulus(kanooth_integer& r, const kanooth_integer& a)
{ r.data().modulus_truncate(r.data(), a.data()); }
inline void eval_modulus(kanooth_integer& r, unsigned long a)
{ r.data().modulus_truncate(r.data(), a); }
inline void eval_modulus(kanooth_integer& r, signed long a)
{ r.data().modulus_truncate(r.data(), a); }
inline void eval_modulus(kanooth_integer& r, const kanooth_integer& a, const kanooth_integer& b)
{ r.data().modulus_truncate(a.data(), b.data()); }
inline void eval_modulus(kanooth_integer& r, const kanooth_integer& a, unsigned long b)
{ r.data().modulus_truncate(a.data(), b); }
inline void eval_modulus(kanooth_integer& r, const kanooth_integer& a, signed long b)
{ r.data().modulus_truncate(a.data(), b); }

inline void eval_qr(const kanooth_integer& x, const kanooth_integer& y, kanooth_integer& q, kanooth_integer& r) {
    kanooth_integer::data_type::quotrem(q.data(), r.data(), x.data(), y.data());
}

template <class Integer>
inline typename enable_if<is_unsigned<Integer>, Integer>::type eval_integer_modulus(const kanooth_integer& x, Integer val)
{
    if ((sizeof(Integer) <= sizeof(long)) || (val <= (std::numeric_limits<unsigned long>::max)())) {
        return kanooth_integer::data_type::integer_modulus(x.data(), static_cast<unsigned long>(val));
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
   result.data().bitwise_and(result.data(), v.data());
}

inline void eval_bitwise_or(kanooth_integer& result, const kanooth_integer& v)
{
   result.data().bitwise_or(result.data(), v.data());
}

inline void eval_bitwise_xor(kanooth_integer& result, const kanooth_integer& v)
{
   result.data().bitwise_xor(result.data(), v.data());
}

inline void eval_bitwise_and(kanooth_integer& result, const kanooth_integer& u, const kanooth_integer& v)
{
   result.data().bitwise_and(u.data(), v.data());
}

inline void eval_bitwise_or(kanooth_integer& result, const kanooth_integer& u, const kanooth_integer& v)
{
   result.data().bitwise_or(u.data(), v.data());
}

inline void eval_bitwise_xor(kanooth_integer& result, const kanooth_integer& u, const kanooth_integer& v)
{
   result.data().bitwise_xor(u.data(), v.data());
}

inline void eval_left_shift(kanooth_integer& r, unsigned long v)
{
   r.data().left_shift(r.data(), v);
}

inline void eval_left_shift(kanooth_integer& r, kanooth_integer& u, unsigned long v)
{
   r.data().left_shift(u.data(), v);
}

inline void eval_right_shift(kanooth_integer& r, unsigned long v)
{
   r.data().right_shift(r.data(), v);
}

inline void eval_right_shift(kanooth_integer& r, kanooth_integer& u, unsigned long v)
{
   r.data().right_shift(u.data(), v);
}

inline bool eval_eq(const kanooth_integer& a, const kanooth_integer& b)
{ return a.data().compare(b.data()) == 0; }

template <class T>
inline typename enable_if<mpl::contains<kanooth_integer::native_types, T>, bool>::type
eval_eq(const kanooth_integer& a, T b)
{ return a.data().compare(b) == 0; }

inline bool eval_lt(const kanooth_integer& a, const kanooth_integer& b)
{ return a.data().compare(b.data()) < 0; }

template <class T>
inline typename enable_if<mpl::contains<kanooth_integer::native_types, T>, bool>::type
eval_lt(const kanooth_integer& a, T b)
{ return a.data().compare(b) < 0; }

inline bool eval_gt(const kanooth_integer& a, const kanooth_integer& b)
{ return a.data().compare(b.data()) > 0; }
template <class T>
inline typename enable_if<mpl::contains<kanooth_integer::native_types, T>, bool>::type
eval_gt(const kanooth_integer& a, T b)
{ return a.data().compare(b) > 0; }

} // namespace backends
} // namespace multiprecision
} // namespace boost

namespace kanooth {
namespace numbers {

typedef boost::multiprecision::number<boost::multiprecision::backends::kanooth_integer> boost_integer;

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_BOOST_INTEGER_HPP
