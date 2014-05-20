/*
 * Author: Jan Marthedal Rasmussen
 * Created 2014-05-20
 */

#ifndef DEMO_COMMON_HPP
#define DEMO_COMMON_HPP

template <typename T>
T gcd(const T& a, const T& b)
{
    T r;
    r.gcd(a, b);
    return r;
}

template <typename T>
T power(T y, T x, unsigned n) {
  if (n) {
    while (n > 1) {
      if (n & 1) y.multiply(y, x);
      x.multiply(x, x);
      n >>= 1;
    }
    y.multiply(y, x);
  }
  return y;
}

#endif // DEMO_COMMON_HPP
