#include <vector>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <kanooth/numbers/lowlevel/generic_has_double.hpp>
#include <kanooth/numbers/lowlevel/generic_sim_double.hpp>
#include <kanooth/numbers/lowlevel/low_double_int.hpp>
#include <kanooth/number_bits.hpp>
#include <kanooth/fixed_width_ints.hpp>

#include "common/stopwatch.hpp"

boost::random::mt19937 gen;

template <typename T>
void random_array(T* begin, T* end)
{
    boost::random::uniform_int_distribution<T> rand;
    while (begin != end)
        *begin++ = rand(gen);
}

template <typename LOWLEVEL>
class tester {
public:
    tester(unsigned _length) : length(_length)
    {
        digit_type* r;
        for (unsigned k = 0; k < count; ++k) {
            r = new digit_type[length];
            random_array(r, r + length);
            a.push_back(r);
            r = new digit_type[length];
            random_array(r, r + length);
            b.push_back(r);
            c.push_back(new digit_type[length]);
        }
        random_array(&scalar, &scalar + 1);
    }
    
    ~tester()
    {
        for (unsigned k = 0; k < count; ++k) {
            delete[] a[k];
            delete[] b[k];
            delete[] c[k];
        }
    }
    
    double test_add()
    {
        stopwatch<> timer;

        unsigned reps = length > 10000 ? 1 : 10000/length;
        for (unsigned j = 0; j < reps; ++j)
            for (unsigned k = 0; k < count; ++k)
                LOWLEVEL::add(c[k], a[k], length, b[k], length);        
        
        return 0.000000001 * count * reps * length * digit_bits / timer.seconds();
    }
    
    double test_sub()
    {
        stopwatch<> timer;

        unsigned reps = length > 10000 ? 1 : 10000/length;
        for (unsigned j = 0; j < reps; ++j)
            for (unsigned k = 0; k < count; ++k)
                LOWLEVEL::sub(c[k], a[k], length, b[k], length);        
        
        return 0.000000001 * count * reps * length * digit_bits / timer.seconds();
    }
    
    double test_mul_1()
    {
        stopwatch<> timer;

        unsigned reps = length > 10000 ? 1 : 10000/length;
        for (unsigned j = 0; j < reps; ++j)
            for (unsigned k = 0; k < count; ++k)
                LOWLEVEL::mul_1(c[k], a[k], length, b[k][0]);
        
        return 0.000000001 * count * reps * length * digit_bits / timer.seconds();
    }
    
    void run()
    {
        std::cout.flags(std::ios::fixed);
        std::cout.precision(2);
        std::cout << "add       " << test_add() << " Gbit/s" << std::endl;
        std::cout << "sub       " << test_sub() << " Gbit/s" << std::endl;
        std::cout << "mul_1     " << test_mul_1() << " Gbit/s" << std::endl;
    }
private:
    typedef typename LOWLEVEL::digit_type digit_type;
    static const unsigned digit_bits = kanooth::number_bits<digit_type>::value;
    static const unsigned count = 10000;
    const unsigned length;
    std::vector<digit_type*> a;
    std::vector<digit_type*> b;
    std::vector<digit_type*> c;
    digit_type scalar;
};

int main()
{
    //tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint64_t, kanooth::uint128_t> >(1000).run();
    tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint32_t, kanooth::uint64_t> >(1000).run();
    //tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint16_t, kanooth::uint32_t> >(1000).run();
    //tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint64_t, kanooth::numbers::lowlevel::low_double_int<kanooth::uint64_t> > >(1000).run();
    tester<kanooth::numbers::lowlevel::generic_sim_double<kanooth::uint64_t> >(1000).run();
    
    /*tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint16_t, kanooth::uint32_t> >(1000).run();
    tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint16_t, kanooth::numbers::lowlevel::low_double_int<kanooth::uint16_t> > >(1000).run();
    tester<kanooth::numbers::lowlevel::generic_sim_double<kanooth::uint16_t> >(1000).run();*/

    /*tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint64_t, kanooth::uint128_t> >(1000).run();
    tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint64_t, kanooth::numbers::lowlevel::low_double_int<kanooth::uint64_t> > >(1000).run();
    tester<kanooth::numbers::lowlevel::generic_sim_double<kanooth::uint64_t> >(1000).run();*/

    return 0;
}
