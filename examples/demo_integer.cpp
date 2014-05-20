/*
 * Author: Jan Marthedal Rasmussen
 * Created 2014-05-08
 */

#include <iostream>
#include <kanooth/numbers/integer.hpp>

using kanooth::numbers::integer;

int main()
{
    integer i = -12;
    
    std::cout << i.str() << std::endl;
    
    return 0;
}

