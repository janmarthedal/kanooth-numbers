#ifndef BOOST_MATH_ER_GMP_BACKEND_HPP
#define BOOST_MATH_ER_GMP_BACKEND_HPP

#include <boost/multiprecision/number.hpp>
#include <boost/multiprecision/detail/integer_ops.hpp>
#include <boost/multiprecision/detail/digits.hpp>
#include <boost/cstdint.hpp>
#include <boost/mpl/contains.hpp>

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
    kanooth_integer(const kanooth_integer& o) : m_data(o.m_data) {}
    kanooth_integer& operator = (const kanooth_integer& o) {
        m_data = o.m_data;
        return *this;
    }
    kanooth_integer& operator = (long i) {
        m_data = i;
        return *this;
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


/*
inline void eval_divide(kanooth_integer& t, const kanooth_integer& o)
{
    if (eval_is_zero(o))
        BOOST_THROW_EXCEPTION(std::overflow_error("Division by zero."));
    t.data().divide(o.data());
}
inline void eval_modulus(kanooth_integer& t, const kanooth_integer& o)
{
    t.data().modulus(o.data());
}
inline void eval_divide(kanooth_integer& t, const kanooth_integer& p, const kanooth_integer& o)
{
   if(eval_is_zero(o))
      BOOST_THROW_EXCEPTION(std::overflow_error("Division by zero."));
   t.data().divide(p.data(), o.data());
}
inline void eval_modulus(kanooth_integer& t, const kanooth_integer& p, const kanooth_integer& o)
{
   t.data().modulus(p.data(), o.data());
}
*/

inline bool eval_eq(const kanooth_integer& a, const kanooth_integer& b)
{
   return a.data().compare(b.data()) == 0;
}
template <class T>
inline typename enable_if<mpl::contains<kanooth_integer::native_types, T>, bool>::type
eval_eq(const kanooth_integer& a, T b)
{
   return a.data().compare(b) == 0;
}

inline bool eval_lt(const kanooth_integer& a, const kanooth_integer& b)
{
   return a.data().compare(b.data()) < 0;
}
template <class T>
inline typename enable_if<mpl::contains<kanooth_integer::native_types, T>, bool>::type
eval_lt(const kanooth_integer& a, T b)
{
   return a.data().compare(b) < 0;
}

inline bool eval_gt(const kanooth_integer& a, const kanooth_integer& b)
{
   return a.data().compare(b.data()) > 0;
}
template <class T>
inline typename enable_if<mpl::contains<kanooth_integer::native_types, T>, bool>::type
eval_gt(const kanooth_integer& a, T b)
{
   return a.data().compare(b) > 0;
}


} //namespace backends

typedef number<backends::kanooth_integer> boost_integer;

}  // namespace multiprecision
}  // namespace boost


#endif
