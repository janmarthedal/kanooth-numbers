#ifndef KANOOTH_TEST_STOPWATCH_HPP
#define KANOOTH_TEST_STOPWATCH_HPP

#define BOOST_ERROR_CODE_HEADER_ONLY
#define BOOST_CHRONO_HEADER_ONLY
#include <boost/chrono.hpp>

template <class Clock = boost::chrono::high_resolution_clock>
struct stopwatch
{
   stopwatch()
   {
      m_start = Clock::now();
   }
   double elapsed()
   {
      return boost::chrono::duration_cast<boost::chrono::duration<double> >(Clock::now() - m_start).count();
   }
   void reset()
   {
      m_start = Clock::now();
   }

private:
   typename Clock::time_point m_start;
};

#endif // KANOOTH_TEST_STOPWATCH_HPP
