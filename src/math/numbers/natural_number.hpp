#ifndef _SPUTSOFT_MATH_NUMBERS_NATURAL_NUMBER_HPP_
#define _SPUTSOFT_MATH_NUMBERS_NATURAL_NUMBER_HPP_

namespace sputsoft {
namespace math {
namespace numbers {

template <typename Con> //, typename LowLevel>
class natural_number {
  template <typename Os, typename C>
  friend Os& operator<<(Os& os, const natural_number<C>& n);
private:
  Con digits;
public:
  natural_number() : digits() {}
  natural_number(unsigned v) : digits(1) {
    if (v) {
      digits[0] = v;
      digits.set_size(1);
    }
  }
};

template <typename Os, typename Con>
Os& operator<<(Os& os, const natural_number<Con>& n)
{
  return os << n.digits;
}

} // namespace numbers
} // namespace math
} // namespace sputsoft

#endif // _SPUTSOFT_MATH_NUMBERS_NATURAL_NUMBER_HPP_

