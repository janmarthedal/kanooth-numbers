#ifndef KANOOTH_DETAIL_SET_TYPE_HPP
#define KANOOTH_DETAIL_SET_TYPE_HPP

namespace kanooth {
namespace detail {

template <typename T>
struct set_type {
    static const bool enabled = true;
    typedef T type;
};

} // namespace detail
} // namespace kanooth

#endif // KANOOTH_DETAIL_SET_TYPE_HPP