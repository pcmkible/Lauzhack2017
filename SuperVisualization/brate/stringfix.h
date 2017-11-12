#ifndef INCLUDE_STRINGFIX_H
#define INCLUDE_STRINGFIX_H

#include<iostream>
#include<iomanip>
#include<string>
#include<sstream>

//Ya que no hay std::int-to-string
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << std::fixed << std::setprecision(2) << n ;
        return stm.str() ;
    }
}

#endif // INCLUDE_STRINGFIX_H
