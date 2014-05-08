#ifndef KANOOTH_SIGNED_RIGHT_SHIFT_HPP
#define KANOOTH_SIGNED_RIGHT_SHIFT_HPP

#include <kanooth/make_signed.hpp>

namespace kanooth {

namespace detail {
    
    template <typename T>
    struct signed_right_shift_eval {
        T operator()(const T v, unsigned amount) const {
            return static_cast<typename make_signed<T>::type>(v) >> amount;    
        }
    };

} // namespace detail

template <typename T>
T signed_right_shift(const T v, unsigned amount)
{
    return detail::signed_right_shift_eval<T>()(v, amount);
}

} // namespace kanooth

#endif // KANOOTH_SIGNED_RIGHT_SHIFT_HPP
