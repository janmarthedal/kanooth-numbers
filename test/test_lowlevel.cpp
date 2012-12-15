#include <vector>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <kanooth/numbers/lowlevel/generic_has_double.hpp>
#include <kanooth/number_bits.hpp>
#include <kanooth/fixed_width_ints.hpp>

#include "common/stopwatch.hpp"

boost::random::mt19937 gen;

template <typename T>
void random_array(T* begin, T* end)
{
    boost::random::uniform_int_distribution<T> rand;
    while (begin != end)
        *begin++ = rand();
}

template <typename LOWLEVEL>
class tester {
public:
    tester(unsigned _length) : length(_length)
    {
        for (unsigned k = 0; k < count; ++k) {
            a.push_back(new digit_type[length]);
            b.push_back(new digit_type[length]);
            c.push_back(new digit_type[length]);
        }
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
        
        return 1.0 * count * reps * length * digit_bits / timer.elapsed();
    }
    
    void run()
    {
        std::cout.flags(std::ios::fixed);
        std::cout.precision(0);
        std::cout << "add       " << test_add() << std::endl;
    }
private:
    typedef typename LOWLEVEL::digit_type digit_type;
    static const unsigned digit_bits = kanooth::number_bits<digit_type>::value;
    static const unsigned count = 10000;
    const unsigned length;
    std::vector<digit_type*> a;
    std::vector<digit_type*> b;
    std::vector<digit_type*> c;
};

int main()
{
    //tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint32_t, kanooth::uint64_t> > test(1000);
    tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint16_t, kanooth::uint32_t> > test(1000);
    test.run();
    
    return 0;
}
