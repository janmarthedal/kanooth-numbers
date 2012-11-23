#include <boost/multiprecision/gmp.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <kanooth/numbers/boost_integer.hpp>

template <class T>
T generate_random(unsigned bits_wanted)
{
   static boost::random::mt19937 gen;
   typedef boost::random::mt19937::result_type random_type;

   T max_val;
   unsigned digits;
   if(std::numeric_limits<T>::is_bounded && (bits_wanted == std::numeric_limits<T>::digits))
   {
      max_val = (std::numeric_limits<T>::max)();
      digits = std::numeric_limits<T>::digits;
   }
   else
   {
      max_val = T(1) << bits_wanted;
      digits = bits_wanted;
   }

   unsigned bits_per_r_val = std::numeric_limits<random_type>::digits - 1;
   while((random_type(1) << bits_per_r_val) > (gen.max)()) --bits_per_r_val;

   unsigned terms_needed = digits / bits_per_r_val + 1;

   T val = 0;
   for(unsigned i = 0; i < terms_needed; ++i)
   {
      val *= (gen.max)();
      val += gen();
   }
   val %= max_val;
   return val;
}


template <class Number>
struct tester
{
   typedef Number test_type;

   unsigned last_error_count;

   boost::multiprecision::mpz_int a, b, c, d;
   int si;
   unsigned ui;
   test_type a1, b1, c1, d1;

   void t1()
   {
      using namespace boost::multiprecision;
      BOOST_TEST_EQ(a.str(), a1.str());
      BOOST_TEST_EQ(b.str(), b1.str());
      BOOST_TEST_EQ(c.str(), c1.str());
      BOOST_TEST_EQ(d.str(), d1.str());
      BOOST_TEST_EQ(mpz_int(a+b).str(), test_type(a1 + b1).str());
      BOOST_TEST_EQ((mpz_int(a)+=b).str(), (test_type(a1) += b1).str());
      BOOST_TEST_EQ((mpz_int(b)+=a).str(), (test_type(b1) += a1).str());
      BOOST_TEST_EQ(mpz_int(a-b).str(), test_type(a1 - b1).str());
      BOOST_TEST_EQ(mpz_int(mpz_int(-a)+b).str(), test_type(test_type(-a1) + b1).str());
      BOOST_TEST_EQ(mpz_int(mpz_int(-a)-b).str(), test_type(test_type(-a1) - b1).str());
      BOOST_TEST_EQ(mpz_int(c * d).str(), test_type(c1 * d1).str());
      BOOST_TEST_EQ((mpz_int(c)*=d).str(), (test_type(c1) *= d1).str());
      BOOST_TEST_EQ((mpz_int(d)*=c).str(), (test_type(d1) *= c1).str());
      BOOST_TEST_EQ(mpz_int(c * -d).str(), test_type(c1 * -d1).str());
      BOOST_TEST_EQ(mpz_int(-c * d).str(), test_type(-c1 * d1).str());
      BOOST_TEST_EQ((mpz_int(c)*=-d).str(), (test_type(c1) *= -d1).str());
      BOOST_TEST_EQ((mpz_int(-d)*=c).str(), (test_type(-d1) *= c1).str());
      BOOST_TEST_EQ(mpz_int(b * c).str(), test_type(b1 * c1).str());
      BOOST_TEST_EQ(mpz_int(a / b).str(), test_type(a1 / b1).str());
      BOOST_TEST_EQ((mpz_int(a)/=b).str(), (test_type(a1) /= b1).str());
      BOOST_TEST_EQ(mpz_int(a / -b).str(), test_type(a1 / -b1).str());
      BOOST_TEST_EQ(mpz_int(-a / b).str(), test_type(-a1 / b1).str());
      BOOST_TEST_EQ((mpz_int(a)/=-b).str(), (test_type(a1) /= -b1).str());
      BOOST_TEST_EQ((mpz_int(-a)/=b).str(), (test_type(-a1) /= b1).str());
      BOOST_TEST_EQ(mpz_int(a / d).str(), test_type(a1 / d1).str());
      BOOST_TEST_EQ(mpz_int(a % b).str(), test_type(a1 % b1).str());
      BOOST_TEST_EQ((mpz_int(a)%=b).str(), (test_type(a1) %= b1).str());
      BOOST_TEST_EQ(mpz_int(a % -b).str(), test_type(a1 % -b1).str());
      BOOST_TEST_EQ((mpz_int(a)%=-b).str(), (test_type(a1) %= -b1).str());
      BOOST_TEST_EQ(mpz_int(-a % b).str(), test_type(-a1 % b1).str());
      BOOST_TEST_EQ((mpz_int(-a)%=b).str(), (test_type(-a1) %= b1).str());
      BOOST_TEST_EQ(mpz_int(a % d).str(), test_type(a1 % d1).str());
      BOOST_TEST_EQ((mpz_int(a)%=d).str(), (test_type(a1) %= d1).str());
   }

   void t2()
   {
      using namespace boost::multiprecision;
      // bitwise ops:
      BOOST_TEST_EQ(mpz_int(a|b).str(), test_type(a1 | b1).str());
      BOOST_TEST_EQ((mpz_int(a)|=b).str(), (test_type(a1) |= b1).str());
         BOOST_TEST_EQ(mpz_int(-a|b).str(), test_type(-a1 | b1).str());
         BOOST_TEST_EQ((mpz_int(-a)|=b).str(), (test_type(-a1) |= b1).str());
         BOOST_TEST_EQ(mpz_int(a|-b).str(), test_type(a1 | -b1).str());
         BOOST_TEST_EQ((mpz_int(a)|=-b).str(), (test_type(a1) |= -b1).str());
         BOOST_TEST_EQ(mpz_int(-a|-b).str(), test_type(-a1 | -b1).str());
         BOOST_TEST_EQ((mpz_int(-a)|=-b).str(), (test_type(-a1) |= -b1).str());
      BOOST_TEST_EQ(mpz_int(a&b).str(), test_type(a1 & b1).str());
      BOOST_TEST_EQ((mpz_int(a)&=b).str(), (test_type(a1) &= b1).str());
         BOOST_TEST_EQ(mpz_int(-a&b).str(), test_type(-a1 & b1).str());
         BOOST_TEST_EQ((mpz_int(-a)&=b).str(), (test_type(-a1) &= b1).str());
         BOOST_TEST_EQ(mpz_int(a&-b).str(), test_type(a1 & -b1).str());
         BOOST_TEST_EQ((mpz_int(a)&=-b).str(), (test_type(a1) &= -b1).str());
         BOOST_TEST_EQ(mpz_int(-a&-b).str(), test_type(-a1 & -b1).str());
         BOOST_TEST_EQ((mpz_int(-a)&=-b).str(), (test_type(-a1) &= -b1).str());
      BOOST_TEST_EQ(mpz_int(a^b).str(), test_type(a1 ^ b1).str());
      BOOST_TEST_EQ((mpz_int(a)^=b).str(), (test_type(a1) ^= b1).str());
         BOOST_TEST_EQ(mpz_int(-a^b).str(), test_type(-a1 ^ b1).str());
         BOOST_TEST_EQ((mpz_int(-a)^=b).str(), (test_type(-a1) ^= b1).str());
         BOOST_TEST_EQ(mpz_int(a^-b).str(), test_type(a1 ^ -b1).str());
         BOOST_TEST_EQ((mpz_int(a)^=-b).str(), (test_type(a1) ^= -b1).str());
         BOOST_TEST_EQ(mpz_int(-a^-b).str(), test_type(-a1 ^ -b1).str());
         BOOST_TEST_EQ((mpz_int(-a)^=-b).str(), (test_type(-a1) ^= -b1).str());
      // Shift ops:
      for(unsigned i = 0; i < 128; ++i)
      {
        BOOST_TEST_EQ(mpz_int(a << i).str(), test_type(a1 << i).str());
         BOOST_TEST_EQ(mpz_int(a >> i).str(), test_type(a1 >> i).str());
      }
      // gcd/lcm
      BOOST_TEST_EQ(mpz_int(gcd(a, b)).str(), test_type(gcd(a1, b1)).str());
      BOOST_TEST_EQ(mpz_int(lcm(c, d)).str(), test_type(lcm(c1, d1)).str());
      BOOST_TEST_EQ(mpz_int(gcd(-a, b)).str(), test_type(gcd(-a1, b1)).str());
      BOOST_TEST_EQ(mpz_int(lcm(-c, d)).str(), test_type(lcm(-c1, d1)).str());
      BOOST_TEST_EQ(mpz_int(gcd(-a, -b)).str(), test_type(gcd(-a1, -b1)).str());
      BOOST_TEST_EQ(mpz_int(lcm(-c, -d)).str(), test_type(lcm(-c1, -d1)).str());
      BOOST_TEST_EQ(mpz_int(gcd(a, -b)).str(), test_type(gcd(a1, -b1)).str());
      BOOST_TEST_EQ(mpz_int(lcm(c, -d)).str(), test_type(lcm(c1, -d1)).str());
   }

   void t3()
   {
      using namespace boost::multiprecision;
      // Now check operations involving signed integers:
      BOOST_TEST_EQ(mpz_int(a + si).str(), test_type(a1 + si).str());
      BOOST_TEST_EQ(mpz_int(a + -si).str(), test_type(a1 + -si).str());
      BOOST_TEST_EQ(mpz_int(-a + si).str(), test_type(-a1 + si).str());
      BOOST_TEST_EQ(mpz_int(si + a).str(), test_type(si + a1).str());
      BOOST_TEST_EQ((mpz_int(a)+=si).str(), (test_type(a1) += si).str());
      BOOST_TEST_EQ((mpz_int(a)+=-si).str(), (test_type(a1) += -si).str());
      BOOST_TEST_EQ((mpz_int(-a)+=si).str(), (test_type(-a1) += si).str());
      BOOST_TEST_EQ((mpz_int(-a)+=-si).str(), (test_type(-a1) += -si).str());
      BOOST_TEST_EQ(mpz_int(a - si).str(), test_type(a1 - si).str());
      BOOST_TEST_EQ(mpz_int(a - -si).str(), test_type(a1 - -si).str());
      BOOST_TEST_EQ(mpz_int(-a - si).str(), test_type(-a1 - si).str());
      BOOST_TEST_EQ(mpz_int(si - a).str(), test_type(si - a1).str());
      BOOST_TEST_EQ((mpz_int(a)-=si).str(), (test_type(a1) -= si).str());
      BOOST_TEST_EQ((mpz_int(a)-=-si).str(), (test_type(a1) -= -si).str());
      BOOST_TEST_EQ((mpz_int(-a)-=si).str(), (test_type(-a1) -= si).str());
      BOOST_TEST_EQ((mpz_int(-a)-=-si).str(), (test_type(-a1) -= -si).str());
      BOOST_TEST_EQ(mpz_int(b * si).str(), test_type(b1 * si).str());
      BOOST_TEST_EQ(mpz_int(b * -si).str(), test_type(b1 * -si).str());
      BOOST_TEST_EQ(mpz_int(-b * si).str(), test_type(-b1 * si).str());
      BOOST_TEST_EQ(mpz_int(si * b).str(), test_type(si * b1).str());
      BOOST_TEST_EQ((mpz_int(a)*=si).str(), (test_type(a1) *= si).str());
      BOOST_TEST_EQ((mpz_int(a)*=-si).str(), (test_type(a1) *= -si).str());
      BOOST_TEST_EQ((mpz_int(-a)*=si).str(), (test_type(-a1) *= si).str());
      BOOST_TEST_EQ((mpz_int(-a)*=-si).str(), (test_type(-a1) *= -si).str());
      BOOST_TEST_EQ(mpz_int(a / si).str(), test_type(a1 / si).str());
      BOOST_TEST_EQ(mpz_int(a / -si).str(), test_type(a1 / -si).str());
      BOOST_TEST_EQ(mpz_int(-a / si).str(), test_type(-a1 / si).str());
      BOOST_TEST_EQ((mpz_int(a)/=si).str(), (test_type(a1) /= si).str());
      BOOST_TEST_EQ((mpz_int(a)/=-si).str(), (test_type(a1) /= -si).str());
      BOOST_TEST_EQ((mpz_int(-a)/=si).str(), (test_type(-a1) /= si).str());
      BOOST_TEST_EQ((mpz_int(-a)/=-si).str(), (test_type(-a1) /= -si).str());
      BOOST_TEST_EQ(mpz_int(a % si).str(), test_type(a1 % si).str());
      BOOST_TEST_EQ(mpz_int(a % -si).str(), test_type(a1 % -si).str());
      BOOST_TEST_EQ(mpz_int(-a % si).str(), test_type(-a1 % si).str());
      BOOST_TEST_EQ((mpz_int(a)%=si).str(), (test_type(a1) %= si).str());
      BOOST_TEST_EQ((mpz_int(a)%=-si).str(), (test_type(a1) %= -si).str());
      BOOST_TEST_EQ((mpz_int(-a)%=si).str(), (test_type(-a1) %= si).str());
      BOOST_TEST_EQ((mpz_int(-a)%=-si).str(), (test_type(-a1) %= -si).str());
         BOOST_TEST_EQ(mpz_int(a|si).str(), test_type(a1 | si).str());
         BOOST_TEST_EQ((mpz_int(a)|=si).str(), (test_type(a1) |= si).str());
         BOOST_TEST_EQ(mpz_int(a&si).str(), test_type(a1 & si).str());
         BOOST_TEST_EQ((mpz_int(a)&=si).str(), (test_type(a1) &= si).str());
         BOOST_TEST_EQ(mpz_int(a^si).str(), test_type(a1 ^ si).str());
         BOOST_TEST_EQ((mpz_int(a)^=si).str(), (test_type(a1) ^= si).str());
         BOOST_TEST_EQ(mpz_int(si|a).str(), test_type(si|a1).str());
         BOOST_TEST_EQ(mpz_int(si&a).str(), test_type(si&a1).str());
         BOOST_TEST_EQ(mpz_int(si^a).str(), test_type(si^a1).str());
      BOOST_TEST_EQ(mpz_int(gcd(a, si)).str(), test_type(gcd(a1, si)).str());
      BOOST_TEST_EQ(mpz_int(gcd(si, b)).str(), test_type(gcd(si, b1)).str());
      BOOST_TEST_EQ(mpz_int(lcm(c, si)).str(), test_type(lcm(c1, si)).str());
      BOOST_TEST_EQ(mpz_int(lcm(si, d)).str(), test_type(lcm(si, d1)).str());
      BOOST_TEST_EQ(mpz_int(gcd(-a, si)).str(), test_type(gcd(-a1, si)).str());
      BOOST_TEST_EQ(mpz_int(gcd(-si, b)).str(), test_type(gcd(-si, b1)).str());
      BOOST_TEST_EQ(mpz_int(lcm(-c, si)).str(), test_type(lcm(-c1, si)).str());
      BOOST_TEST_EQ(mpz_int(lcm(-si, d)).str(), test_type(lcm(-si, d1)).str());
      BOOST_TEST_EQ(mpz_int(gcd(-a, -si)).str(), test_type(gcd(-a1, -si)).str());
      BOOST_TEST_EQ(mpz_int(gcd(-si, -b)).str(), test_type(gcd(-si, -b1)).str());
      BOOST_TEST_EQ(mpz_int(lcm(-c, -si)).str(), test_type(lcm(-c1, -si)).str());
      BOOST_TEST_EQ(mpz_int(lcm(-si, -d)).str(), test_type(lcm(-si, -d1)).str());
      BOOST_TEST_EQ(mpz_int(gcd(a, -si)).str(), test_type(gcd(a1, -si)).str());
      BOOST_TEST_EQ(mpz_int(gcd(si, -b)).str(), test_type(gcd(si, -b1)).str());
      BOOST_TEST_EQ(mpz_int(lcm(c, -si)).str(), test_type(lcm(c1, -si)).str());
      BOOST_TEST_EQ(mpz_int(lcm(si, -d)).str(), test_type(lcm(si, -d1)).str());
   }

   void t4()
   {
      using namespace boost::multiprecision;
      // Now check operations involving unsigned integers:
      BOOST_TEST_EQ(mpz_int(a + ui).str(), test_type(a1 + ui).str());
      BOOST_TEST_EQ(mpz_int(-a + ui).str(), test_type(-a1 + ui).str());
      BOOST_TEST_EQ(mpz_int(ui + a).str(), test_type(ui + a1).str());
      BOOST_TEST_EQ((mpz_int(a)+=ui).str(), (test_type(a1) += ui).str());
      BOOST_TEST_EQ((mpz_int(-a)+=ui).str(), (test_type(-a1) += ui).str());
      BOOST_TEST_EQ(mpz_int(a - ui).str(), test_type(a1 - ui).str());
      BOOST_TEST_EQ(mpz_int(-a - ui).str(), test_type(-a1 - ui).str());
      BOOST_TEST_EQ(mpz_int(ui - a).str(), test_type(ui - a1).str());
      BOOST_TEST_EQ((mpz_int(a)-=ui).str(), (test_type(a1) -= ui).str());
      BOOST_TEST_EQ((mpz_int(-a)-=ui).str(), (test_type(-a1) -= ui).str());
      BOOST_TEST_EQ(mpz_int(b * ui).str(), test_type(b1 * ui).str());
      BOOST_TEST_EQ(mpz_int(-b * ui).str(), test_type(-b1 * ui).str());
      BOOST_TEST_EQ(mpz_int(ui * b).str(), test_type(ui * b1).str());
      BOOST_TEST_EQ((mpz_int(a)*=ui).str(), (test_type(a1) *= ui).str());
      BOOST_TEST_EQ((mpz_int(-a)*=ui).str(), (test_type(-a1) *= ui).str());
      BOOST_TEST_EQ(mpz_int(a / ui).str(), test_type(a1 / ui).str());
      BOOST_TEST_EQ(mpz_int(-a / ui).str(), test_type(-a1 / ui).str());
      BOOST_TEST_EQ((mpz_int(a)/=ui).str(), (test_type(a1) /= ui).str());
      BOOST_TEST_EQ((mpz_int(-a)/=ui).str(), (test_type(-a1) /= ui).str());
      BOOST_TEST_EQ(mpz_int(a % ui).str(), test_type(a1 % ui).str());
      BOOST_TEST_EQ(mpz_int(-a % ui).str(), test_type(-a1 % ui).str());
      BOOST_TEST_EQ((mpz_int(a)%=ui).str(), (test_type(a1) %= ui).str());
      BOOST_TEST_EQ((mpz_int(-a)%=ui).str(), (test_type(-a1) %= ui).str());
      BOOST_TEST_EQ(mpz_int(a|ui).str(), test_type(a1 | ui).str());
      BOOST_TEST_EQ((mpz_int(a)|=ui).str(), (test_type(a1) |= ui).str());
      BOOST_TEST_EQ(mpz_int(a&ui).str(), test_type(a1 & ui).str());
      BOOST_TEST_EQ((mpz_int(a)&=ui).str(), (test_type(a1) &= ui).str());
      BOOST_TEST_EQ(mpz_int(a^ui).str(), test_type(a1 ^ ui).str());
      BOOST_TEST_EQ((mpz_int(a)^=ui).str(), (test_type(a1) ^= ui).str());
      BOOST_TEST_EQ(mpz_int(ui|a).str(), test_type(ui|a1).str());
      BOOST_TEST_EQ(mpz_int(ui&a).str(), test_type(ui&a1).str());
      BOOST_TEST_EQ(mpz_int(ui^a).str(), test_type(ui^a1).str());
      BOOST_TEST_EQ(mpz_int(gcd(a, ui)).str(), test_type(gcd(a1, ui)).str());
      BOOST_TEST_EQ(mpz_int(gcd(ui, b)).str(), test_type(gcd(ui, b1)).str());
      BOOST_TEST_EQ(mpz_int(lcm(c, ui)).str(), test_type(lcm(c1, ui)).str());
      BOOST_TEST_EQ(mpz_int(lcm(ui, d)).str(), test_type(lcm(ui, d1)).str());
      BOOST_TEST_EQ(mpz_int(gcd(-a, ui)).str(), test_type(gcd(-a1, ui)).str());
      BOOST_TEST_EQ(mpz_int(lcm(-c, ui)).str(), test_type(lcm(-c1, ui)).str());
      BOOST_TEST_EQ(mpz_int(gcd(ui, -b)).str(), test_type(gcd(ui, -b1)).str());
      BOOST_TEST_EQ(mpz_int(lcm(ui, -d)).str(), test_type(lcm(ui, -d1)).str());

   }

   void t5()
   {
      using namespace boost::multiprecision;
      //
      // Now integer functions:
      //
      mpz_int z1, z2;
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
      /*{
         // We have to take care that our powers don't grow too large, otherwise this takes "forever",
         // also don't test for modulo types, as these may give a diffferent result from arbitrary
         // precision types:
         BOOST_TEST_EQ(mpz_int(pow(d, ui % 19)).str(), test_type(pow(d1, ui % 19)).str());
         BOOST_TEST_EQ(mpz_int(powm(a, b, c)).str(), test_type(powm(a1, b1, c1)).str());
         BOOST_TEST_EQ(mpz_int(powm(a, b, ui)).str(), test_type(powm(a1, b1, ui)).str());
         BOOST_TEST_EQ(mpz_int(powm(a, ui, c)).str(), test_type(powm(a1, ui, c1)).str());
      }*/
   }

   void test()
   {
      using namespace boost::multiprecision;

      last_error_count = 0;

      BOOST_TEST_EQ(Number(), 0);

      for(int i = 0; i < 10000; ++i)
      {
         a = generate_random<mpz_int>(1000);
         b = generate_random<mpz_int>(512);
         c = generate_random<mpz_int>(256);
         d = generate_random<mpz_int>(32);

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
            std::cout << std::hex << std::showbase;

            std::cout << "a    = " << a << std::endl;
            std::cout << "a1   = " << a1 << std::endl;
            std::cout << "b    = " << b << std::endl;
            std::cout << "b1   = " << b1 << std::endl;
            std::cout << "c    = " << c << std::endl;
            std::cout << "c1   = " << c1 << std::endl;
            std::cout << "d    = " << d << std::endl;
            std::cout << "d1   = " << d1 << std::endl;
            std::cout << "a + b   = " << a+b << std::endl;
            std::cout << "a1 + b1 = " << a1+b1 << std::endl;
            std::cout << std::dec;
            std::cout << "a - b   = " << a-b << std::endl;
            std::cout << "a1 - b1 = " << a1-b1 << std::endl;
            std::cout << "-a + b   = " << mpz_int(-a)+b << std::endl;
            std::cout << "-a1 + b1 = " << test_type(-a1)+b1 << std::endl;
            std::cout << "-a - b   = " << mpz_int(-a)-b << std::endl;
            std::cout << "-a1 - b1 = " << test_type(-a1)-b1 << std::endl;
            std::cout << "c*d    = " << c*d << std::endl;
            std::cout << "c1*d1  = " << c1*d1 << std::endl;
            std::cout << "b*c    = " << b*c << std::endl;
            std::cout << "b1*c1  = " << b1*c1 << std::endl;
            std::cout << "a/b    = " << a/b << std::endl;
            std::cout << "a1/b1  = " << a1/b1 << std::endl;
            std::cout << "a/d    = " << a/d << std::endl;
            std::cout << "a1/d1  = " << a1/d1 << std::endl;
            std::cout << "a%b    = " << a%b << std::endl;
            std::cout << "a1%b1  = " << a1%b1 << std::endl;
            std::cout << "a%d    = " << a%d << std::endl;
            std::cout << "a1%d1  = " << a1%d1 << std::endl;
         }

      }
   }
};

int main()
{
    typedef kanooth::numbers::boost_integer test_type;
    
    tester<test_type> t1;
    t1.test();

    return boost::report_errors();
}



