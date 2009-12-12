#ifndef _SPUTSOFT_MATH_NUMBERS_DETAIL_DIGIT_ARRAY_HPP_
#define _SPUTSOFT_MATH_NUMBERS_DETAIL_DIGIT_ARRAY_HPP_

namespace sputsoft {
namespace math {
namespace numbers {
namespace detail {

template <typename T>
class digit_array {
  template <typename Os, typename S>
  friend Os& operator<<(Os& os, const digit_array<S>& d);
private:
  T* digits;
  std::size_t used;
  std::size_t allocated;
public:
  digit_array(std::size_t min_size) : used(0) {
    allocated = std::max((std::size_t) 4, min_size);
    digits = new T[allocated];
  }
  ~digit_array() {
    delete[] digits;
  }
  std::size_t size() const { return used; }
  void set_size(std::size_t s) { used = s; }
  const T* get() const { return digits; }
  T* get() { return digits; }
  const T& operator[](std::size_t n) const { return digits[n]; }
  T& operator[](std::size_t n) { return digits[n]; }
};

template <typename Os, typename T>
Os& operator<<(Os& os, const digit_array<T>& d)
{
  os << "[";
  for (std::size_t n=0; n < d.size(); n++) {
    if (n > 0) os << " ";
    os << d[n];
  }
  return os << "]";
}

} // namespace detail
} // namespace numbers
} // namespace math
} // namespace sputsoft

#endif // _SPUTSOFT_MATH_NUMBERS_DETAIL_DIGIT_ARRAY_HPP_

