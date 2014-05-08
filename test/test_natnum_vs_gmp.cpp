#include <boost/multiprecision/gmp.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <kanooth/numbers/boost_natnum.hpp>
#include <kanooth/numbers/generate_random_bits_number.hpp>
#include <kanooth/numbers/integer_binary_logarithm.hpp>
#include <kanooth/numbers/least_significant_bit.hpp>
#include <ctime>
#include <boost/chrono.hpp>

template <class Number>
struct tester
{
    typedef Number test_type;
    typedef boost::multiprecision::mpz_int ref_type;

    boost::random::mt19937 engine;
    unsigned last_error_count;

    ref_type a, b, c, d;
    int si;
    unsigned ui;
    test_type a1, b1, c1, d1;

    void t1()
    {
        BOOST_TEST_EQ(a.str(), a1.str());
        BOOST_TEST_EQ(b.str(), b1.str());
        BOOST_TEST_EQ(c.str(), c1.str());
        BOOST_TEST_EQ(d.str(), d1.str());
        BOOST_TEST_EQ(ref_type(a+b).str(), test_type(a1 + b1).str());
        BOOST_TEST_EQ((ref_type(a)+=b).str(), (test_type(a1) += b1).str());
        BOOST_TEST_EQ((ref_type(b)+=a).str(), (test_type(b1) += a1).str());
        if (a >= b) {
            BOOST_TEST_EQ(ref_type(a-b).str(), test_type(a1 - b1).str());
            BOOST_TEST_EQ((ref_type(a)-=b).str(), (test_type(a1) -= b1).str());
        } else {
            BOOST_TEST_EQ(ref_type(b-a).str(), test_type(b1 - a1).str());
            BOOST_TEST_EQ((ref_type(b)-=a).str(), (test_type(b1) -= a1).str());            
        }
        BOOST_TEST_EQ(ref_type(c * d).str(), test_type(c1 * d1).str());
        BOOST_TEST_EQ((ref_type(c)*=d).str(), (test_type(c1) *= d1).str());
        BOOST_TEST_EQ((ref_type(d)*=c).str(), (test_type(d1) *= c1).str());
        BOOST_TEST_EQ(ref_type(b * c).str(), test_type(b1 * c1).str());
        BOOST_TEST_EQ(ref_type(a / b).str(), test_type(a1 / b1).str());
        BOOST_TEST_EQ((ref_type(a)/=b).str(), (test_type(a1) /= b1).str());
        BOOST_TEST_EQ(ref_type(a / d).str(), test_type(a1 / d1).str());
        BOOST_TEST_EQ(ref_type(a % b).str(), test_type(a1 % b1).str());
        BOOST_TEST_EQ((ref_type(a)%=b).str(), (test_type(a1) %= b1).str());
        BOOST_TEST_EQ(ref_type(a % d).str(), test_type(a1 % d1).str());
        BOOST_TEST_EQ((ref_type(a)%=d).str(), (test_type(a1) %= d1).str());
    }

    void t2()
    {
        // bitwise ops:
        BOOST_TEST_EQ(ref_type(a|b).str(), test_type(a1 | b1).str());
        BOOST_TEST_EQ((ref_type(a)|=b).str(), (test_type(a1) |= b1).str());
        BOOST_TEST_EQ(ref_type(a&b).str(), test_type(a1 & b1).str());
        BOOST_TEST_EQ((ref_type(a)&=b).str(), (test_type(a1) &= b1).str());
        BOOST_TEST_EQ(ref_type(a^b).str(), test_type(a1 ^ b1).str());
        BOOST_TEST_EQ((ref_type(a)^=b).str(), (test_type(a1) ^= b1).str());
        // Shift ops:
        for (unsigned i = 0; i < 128; ++i)
        {
            BOOST_TEST_EQ(ref_type(a << i).str(), test_type(a1 << i).str());
            BOOST_TEST_EQ(ref_type(a >> i).str(), test_type(a1 >> i).str());
        }
        // gcd/lcm
        BOOST_TEST_EQ(ref_type(gcd(a, b)).str(), test_type(gcd(a1, b1)).str());
        BOOST_TEST_EQ(ref_type(lcm(c, d)).str(), test_type(lcm(c1, d1)).str());
    }

    void t3()
    {
        // Now check operations involving signed integers:
        BOOST_TEST_EQ(ref_type(a + si).str(), test_type(a1 + si).str());
        BOOST_TEST_EQ(ref_type(si + a).str(), test_type(si + a1).str());
        BOOST_TEST_EQ((ref_type(a)+=si).str(), (test_type(a1) += si).str());
        if (a >= si) {
            BOOST_TEST_EQ(ref_type(a - si).str(), test_type(a1 - si).str());
            BOOST_TEST_EQ((ref_type(a)-=si).str(), (test_type(a1) -= si).str());
        } else {
            //BOOST_TEST_EQ(ref_type(si - a).str(), test_type(si - a1).str());
        }
        BOOST_TEST_EQ(ref_type(b * si).str(), test_type(b1 * si).str());
        BOOST_TEST_EQ(ref_type(si * b).str(), test_type(si * b1).str());
        BOOST_TEST_EQ((ref_type(a)*=si).str(), (test_type(a1) *= si).str());
        BOOST_TEST_EQ(ref_type(a / si).str(), test_type(a1 / si).str());
        BOOST_TEST_EQ((ref_type(a)/=si).str(), (test_type(a1) /= si).str());
        BOOST_TEST_EQ(ref_type(a % si).str(), test_type(a1 % si).str());
        BOOST_TEST_EQ((ref_type(a)%=si).str(), (test_type(a1) %= si).str());
        BOOST_TEST_EQ(ref_type(a|si).str(), test_type(a1 | si).str());
        BOOST_TEST_EQ((ref_type(a)|=si).str(), (test_type(a1) |= si).str());
        BOOST_TEST_EQ(ref_type(a&si).str(), test_type(a1 & si).str());
        BOOST_TEST_EQ((ref_type(a)&=si).str(), (test_type(a1) &= si).str());
        BOOST_TEST_EQ(ref_type(a^si).str(), test_type(a1 ^ si).str());
        BOOST_TEST_EQ((ref_type(a)^=si).str(), (test_type(a1) ^= si).str());
        BOOST_TEST_EQ(ref_type(si|a).str(), test_type(si|a1).str());
        BOOST_TEST_EQ(ref_type(si&a).str(), test_type(si&a1).str());
        BOOST_TEST_EQ(ref_type(si^a).str(), test_type(si^a1).str());
        BOOST_TEST_EQ(ref_type(gcd(a, si)).str(), test_type(gcd(a1, si)).str());
        BOOST_TEST_EQ(ref_type(gcd(si, b)).str(), test_type(gcd(si, b1)).str());
        BOOST_TEST_EQ(ref_type(lcm(c, si)).str(), test_type(lcm(c1, si)).str());
        BOOST_TEST_EQ(ref_type(lcm(si, d)).str(), test_type(lcm(si, d1)).str());
    }

    void t4()
    {
        // Now check operations involving unsigned integers:
        BOOST_TEST_EQ(ref_type(a + ui).str(), test_type(a1 + ui).str());
        BOOST_TEST_EQ(ref_type(ui + a).str(), test_type(ui + a1).str());
        BOOST_TEST_EQ((ref_type(a)+=ui).str(), (test_type(a1) += ui).str());
        if (a >= ui) {
            BOOST_TEST_EQ(ref_type(a - ui).str(), test_type(a1 - ui).str());
            BOOST_TEST_EQ((ref_type(a)-=ui).str(), (test_type(a1) -= ui).str());
        } else {
            //BOOST_TEST_EQ(ref_type(ui - a).str(), test_type(ui - a1).str());
        }
        BOOST_TEST_EQ(ref_type(b * ui).str(), test_type(b1 * ui).str());
        BOOST_TEST_EQ(ref_type(ui * b).str(), test_type(ui * b1).str());
        BOOST_TEST_EQ((ref_type(a)*=ui).str(), (test_type(a1) *= ui).str());
        BOOST_TEST_EQ(ref_type(a / ui).str(), test_type(a1 / ui).str());
        BOOST_TEST_EQ((ref_type(a)/=ui).str(), (test_type(a1) /= ui).str());
        BOOST_TEST_EQ(ref_type(a % ui).str(), test_type(a1 % ui).str());
        BOOST_TEST_EQ((ref_type(a)%=ui).str(), (test_type(a1) %= ui).str());
        BOOST_TEST_EQ(ref_type(a|ui).str(), test_type(a1 | ui).str());
        BOOST_TEST_EQ((ref_type(a)|=ui).str(), (test_type(a1) |= ui).str());
        BOOST_TEST_EQ(ref_type(a&ui).str(), test_type(a1 & ui).str());
        BOOST_TEST_EQ((ref_type(a)&=ui).str(), (test_type(a1) &= ui).str());
        BOOST_TEST_EQ(ref_type(a^ui).str(), test_type(a1 ^ ui).str());
        BOOST_TEST_EQ((ref_type(a)^=ui).str(), (test_type(a1) ^= ui).str());
        BOOST_TEST_EQ(ref_type(ui|a).str(), test_type(ui|a1).str());
        BOOST_TEST_EQ(ref_type(ui&a).str(), test_type(ui&a1).str());
        BOOST_TEST_EQ(ref_type(ui^a).str(), test_type(ui^a1).str());
        BOOST_TEST_EQ(ref_type(gcd(a, ui)).str(), test_type(gcd(a1, ui)).str());
        BOOST_TEST_EQ(ref_type(gcd(ui, b)).str(), test_type(gcd(ui, b1)).str());
        BOOST_TEST_EQ(ref_type(lcm(c, ui)).str(), test_type(lcm(c1, ui)).str());
        BOOST_TEST_EQ(ref_type(lcm(ui, d)).str(), test_type(lcm(ui, d1)).str());
    }

    void t5()
    {
        //
        // Now integer functions:
        //
        ref_type z1, z2;
        test_type t1, t2;
        divide_qr(a, b, z1, z2);
        divide_qr(a1, b1, t1, t2);
        BOOST_TEST_EQ(z1.str(), t1.str());
        BOOST_TEST_EQ(z2.str(), t2.str());
        BOOST_TEST_EQ(integer_modulus(a, si), integer_modulus(a1, si));
        BOOST_TEST_EQ(lsb(a), lsb(a1));

        for(unsigned i = 0; i < 1000; i += 13)
        {
            BOOST_TEST_EQ(bit_test(a, i), bit_test(a1, i));
        }
        // We have to take care that our powers don't grow too large, otherwise this takes "forever",
        // also don't test for modulo types, as these may give a different result from arbitrary
        // precision types:
        BOOST_TEST_EQ(ref_type(pow(d, ui % 19)).str(), test_type(pow(d1, ui % 19)).str());
        BOOST_TEST_EQ(ref_type(powm(a, b, c)).str(), test_type(powm(a1, b1, c1)).str());
        BOOST_TEST_EQ(ref_type(powm(a, b, ui)).str(), test_type(powm(a1, b1, ui)).str());
        BOOST_TEST_EQ(ref_type(powm(a, ui, c)).str(), test_type(powm(a1, ui, c1)).str());
    }

    void test()
    {
        const unsigned runs = 1000;
        unsigned next_progress_report = 10, progress;
        last_error_count = 0;

        engine.seed(std::time(0));

        BOOST_TEST_EQ(Number(), 0);

        for (unsigned r = 1; r <= runs; ++r)
        {
            progress = 100 * r / runs;
            while (progress >= next_progress_report)
            {
                std::cout << "..." << progress << "%";
                std::cout.flush();
                next_progress_report += 10;
            }

            a = kanooth::numbers::generate_random_bits_number<ref_type>(engine, 1000);
            b = kanooth::numbers::generate_random_bits_number<ref_type>(engine, 512);
            c = kanooth::numbers::generate_random_bits_number<ref_type>(engine, 256);
            d = kanooth::numbers::generate_random_bits_number<ref_type>(engine, 32);

            si = d.convert_to<int>();
            ui = si;

            a1 = static_cast<test_type>(a.str());
            b1 = static_cast<test_type>(b.str());
            c1 = static_cast<test_type>(c.str());
            d1 = static_cast<test_type>(d.str());

            t1();
            t2();
            t3();
            t4();
            t5();

            if(last_error_count != (unsigned)boost::detail::test_errors())
            {
                last_error_count = boost::detail::test_errors();
                //std::cout << std::hex << std::showbase;

                std::cout << "a    = " << a << std::endl;
                std::cout << "a1   = " << a1 << std::endl;
                std::cout << "b    = " << b << std::endl;
                std::cout << "b1   = " << b1 << std::endl;
                std::cout << "c    = " << c << std::endl;
                std::cout << "c1   = " << c1 << std::endl;
                std::cout << "d    = " << d << std::endl;
                std::cout << "d1   = " << d1 << std::endl;
            }

        }
        std::cout << std::endl;
    }
};

int main()
{
    tester<kanooth::numbers::boost_natnum> test_int;
    test_int.test();

    return boost::report_errors();
}
