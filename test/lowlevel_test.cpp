#include <boost/detail/lightweight_test.hpp>
#include <kanooth/numbers/lowlevel/generic_has_double.hpp>
#include <kanooth/numbers/lowlevel/generic_sim_double.hpp>
#include <kanooth/numbers/lowlevel/low_double_int.hpp>

template <typename T>
bool cmp(const T* u, const T* v, unsigned length)
{
    while (length--) {
        if (*u++ != *v++)
            return false;
    }
    return true;
}

template <typename T, typename V>
T* set_digits(T* A, V v1)
{
    A[0] = v1;
    return A;
}

template <typename T, typename V>
T* set_digits(T* A, V v1, V v2)
{
    A[0] = v1;  A[1] = v2;
    return A;
}


template <typename LOWLEVEL>
struct tester {

    typedef typename LOWLEVEL::digit_type digit_type;
    
    digit_type a[20], b[20], r[20], x[20];
    digit_type d;

    void test_add1()
    {
        d = LOWLEVEL::add(r, set_digits(a, -1, 4), 2,
                             set_digits(b,  2),    1);
        BOOST_TEST(cmp(r,    set_digits(x,  1, 5), 2));
        BOOST_TEST_EQ(d, 0);
    }
    
    void test_add2()
    {
        d = LOWLEVEL::add(r, set_digits(a, -3, -1), 2,
                             set_digits(b, -2, -4), 2);
        BOOST_TEST(cmp(r,    set_digits(x, -5, -4), 2));
        BOOST_TEST_EQ(d, 1);
    }
    
    void test_sub1()
    {
        d = LOWLEVEL::sub(r, set_digits(a, 4, 1), 2,
                             set_digits(b, 2),    1);
        BOOST_TEST(cmp(r,    set_digits(x, 2, 1), 2));
        BOOST_TEST_EQ(d, 0);
    }
    
    void test_sub2()
    {
        d = LOWLEVEL::sub(r, set_digits(a,  1,  2), 2,
                             set_digits(b,  3,  5), 2);
        BOOST_TEST(cmp(r,    set_digits(x, -2, -4), 2));
        BOOST_TEST_EQ(d, 1);
    }
    
    void test_mul_1_1()
    {
        d = LOWLEVEL::mul_1(r, set_digits(a, 7, 3), 2,
                               2);
        BOOST_TEST(cmp(r,      set_digits(x, 14, 6), 2));
        BOOST_TEST_EQ(d, 0);
    }
    
    void test_mul_1_2()
    {
        d = LOWLEVEL::mul_1(r, set_digits(a, -1, -1), 2,
                               -1);
        BOOST_TEST(cmp(r,      set_digits(x, 1, -1), 2));
        BOOST_TEST_EQ(d, digit_type(-2));
    }
    
    void run()
    {
        test_add1();
        test_add2();
        test_sub1();
        test_sub2();
        test_mul_1_1();
        test_mul_1_2();
    }
};

int main()
{
    //tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint64_t, kanooth::uint128_t> >().run();
    //tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint64_t, kanooth::numbers::lowlevel::low_double_int<kanooth::uint64_t> > >().run();
    tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint16_t, kanooth::uint32_t> >().run();
    tester<kanooth::numbers::lowlevel::generic_has_double<kanooth::uint16_t, kanooth::numbers::lowlevel::low_double_int<kanooth::uint16_t> > >().run();
    tester<kanooth::numbers::lowlevel::generic_sim_double<kanooth::uint16_t> >().run();
    
    return boost::report_errors();
}