/* 
 * File:   main.cpp
 * Author: jmr
 *
 * Created on May 8, 2014, 11:28 AM
 */

#include <iostream>
#include <kanooth/numbers/natural_number.hpp>

/*
 * 
 */
int main()
{
    typedef kanooth::numbers::natural_number<> num;
    
    num a = "2345687475432563567468467356523543746784";
    num b = 64u;
    num c;

    c.gcd(a, b);

    std::cout << a.to_string() << std::endl;
    std::cout << b.to_string() << std::endl;
    std::cout << c.to_string() << std::endl;

    return 0;
}

