#ifndef KANOOTH_NUMBERS_NATURAL_NUMBER_HPP
#define KANOOTH_NUMBERS_NATURAL_NUMBER_HPP

#include <kanooth/config.hpp>
#include <kanooth/fixed_width_ints.hpp>
#include <kanooth/numbers/natural_number_base.hpp>

#if defined(KANOOTH_HAS_INT128_T)

#include <kanooth/numbers/lowlevel/generic_has_double.hpp>
typedef kanooth::numbers::lowlevel::generic_has_double<kanooth::uint64_t, kanooth::uint128_t> best_low_level;

#elif defined(KANOOTH_HAS_INT64_T)

#include <kanooth/numbers/lowlevel/generic_has_double.hpp>
typedef kanooth::numbers::lowlevel::generic_has_double<kanooth::uint32_t, kanooth::uint64_t> best_low_level;

#else

#include <kanooth/numbers/lowlevel/generic_has_double.hpp>
typedef kanooth::numbers::lowlevel::generic_has_double<kanooth::uint16_t, kanooth::uint32_t> best_low_level;

// for debugging
//#include <kanooth/numbers/lowlevel/generic_has_double.hpp>
//typedef kanooth::numbers::lowlevel::generic_has_double<kanooth::uint8_t, kanooth::uint16_t> best_low_level;
//#include <kanooth/numbers/lowlevel/generic_sim_double.hpp>
//typedef kanooth::numbers::lowlevel::generic_sim_double<unsigned long> best_low_level;

#endif

namespace kanooth {
namespace numbers {

typedef natural_number_base<best_low_level> natural_number;

} // namespace numbers
} // namespace kanooth

#endif // KANOOTH_NUMBERS_NATURAL_NUMBER_HPP
