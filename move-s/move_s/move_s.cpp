#include "fixeds.hpp"
#include "xstr.hpp"
#include "cool_v3.hpp"
//#include "cool_v4.5.hpp"
//#include "cool_v6.hpp"

#include <iostream>
#include <utility>

//using String = FixedSizeString;
using String = cool::Shtring;
//using String = XString;

String make_string(const char* str) {
   String res(str);
   return res;
//   return { str };
}

int main()
{
   String hello = make_string("hello"); 
   String first = String("first");    
   String second;						
   second  =  "second";				
   String third("third");				
   first = third;
#pragma region
   ////first.move_in(third);
#pragma region
   ////first = boost::ref(third);
#pragma region
   //first = std::move(third);
#pragma endregion
#pragma endregion
#pragma endregion
   //std::cout << "I care about only of these strings : " << first.c_str() << ',' << second.c_str() << "..." << std::endl;
   
}

