#include <iostream>

//#include <kanooth/numbers/lowlevel/generic_has_double.hpp>
//#include <kanooth/numbers/lowlevel/generic_sim_double.hpp>
#include <kanooth/fixed_width_ints.hpp>


/*typedef kanooth::uint16_t digit_type;
typedef kanooth::uint32_t double_type;
typedef kanooth::int32_t signed_double_type;
const unsigned digit_bits = 16;*/

typedef kanooth::uint8_t digit_type;
typedef kanooth::uint16_t double_type;
typedef kanooth::int16_t signed_double_type;
const unsigned digit_bits = 8;

void double_mult_add_add(const digit_type u, const digit_type v, const digit_type a1, const digit_type a2, digit_type& low, digit_type& high)
{
  const unsigned int halfbits = digit_bits / 2;
  const digit_type lowmask = (digit_type(1) << halfbits) - 1;
  digit_type u1 = u >> halfbits, u0 = u & lowmask;
  digit_type v1 = v >> halfbits, v0 = v & lowmask;
  digit_type r  = u0*v0 + (a1 & lowmask) + (a2 & lowmask);
  digit_type s  = u1*v0 + (a1 >> halfbits) + (r >> halfbits);
  digit_type t  = u0*v1 + (s & lowmask) + (a2 >> halfbits);
  high = u1*v1 + (s >> halfbits) + (t >> halfbits);
  low  = (t << halfbits) | (r & lowmask);
}

void double_mult_add(const digit_type u, const digit_type v, const digit_type a, digit_type& low, digit_type& high)
{
  double_mult_add_add(u, v, a, digit_type(0), low, high);
}

digit_type mult_sub_sub(digit_type& w, const digit_type u, const digit_type v, digit_type k)
{
  digit_type wt;
  double_mult_add(u, v, k, wt, k);
  if (wt > w) ++k;
  w -= wt;
  return k;
}

digit_type sequence_mult_digit_sub(digit_type* ufirst, digit_type* ulast, const digit_type* vfirst, digit_type q)
{
    digit_type k = 0;
    while (ufirst != ulast) {
        k = mult_sub_sub(*ufirst++, *vfirst++, q, k);
    }
    return k;
}

digit_type sequence_mult_digit_sub2(digit_type* ufirst, digit_type* ulast, const digit_type* vfirst, digit_type q)
{
    double_type k1 = 0, k2 = 0;
    while (ufirst != ulast) {
        k1 += static_cast<double_type>(*vfirst) * static_cast<double_type>(q);
        k2 += static_cast<double_type>(*ufirst) - static_cast<digit_type>(k1);
        *ufirst = static_cast<digit_type>(k2);
        k1 >>= digit_bits;
        k2 = static_cast<signed_double_type>(k2) >> digit_bits;
        ++ufirst;
        ++vfirst;
    }
    return static_cast<digit_type>(k1 - k2);
}

/*digit_type sequence_mult_digit_sub2(digit_type* ufirst, digit_type* ulast, const digit_type* vfirst, digit_type q)
{
    double_type k1 = 0, k2 = 0;
    double_type v1, v2;
    digit_type v3;
    while (ufirst != ulast) {
        v1 = static_cast<double_type>(*ufirst);
        k2 += static_cast<double_type>(*vfirst) * static_cast<double_type>(q);
        v2 = static_cast<double_type>(static_cast<digit_type>(k2));
        k1 += v1 - v2;
        k2 = k2 >> digit_bits;
        v3 = static_cast<digit_type>(k1);
        *ufirst = v3;
        k1 = static_cast<signed_double_type>(k1) >> digit_bits;
        ++ufirst;
        ++vfirst;
    }
    return static_cast<digit_type>(k2 - k1);
}*/


int main()
{
    //typedef kanooth::numbers::lowlevel::generic_has_double<kanooth::uint16, kanooth::uint32_t> lowlevel1;
    
    digit_type a[2] = {-1, -2};
    digit_type b[2] = {-1, -2};
    digit_type m[2] = {-1, -1};
    
    digit_type r1 = sequence_mult_digit_sub(a, a+2, m, -4);
    digit_type r2 = sequence_mult_digit_sub2(b, b+2, m, -4);
    
    //std::cout << a[0] << " " << a[1] << " - " << r1 << std::endl;
    //std::cout << b[0] << " " << b[1] << " - " << r2 << std::endl;
    std::cout << (int)a[0] << " " << (int)a[1] << " - " << (int)r1 << std::endl;
    std::cout << (int)b[0] << " " << (int)b[1] << " - " << (int)r2 << std::endl;
    
    return 0;
}