#include "fixeds.hpp"
#include "cool_v4.hpp"
#include <iostream>
#include <utility>

//using String = FixedSizeString;
using String = cool::Shtring;

String make_string(const char* str) {
   String res(str);
   return res;
}

int main()
{
   //String hello = make_string("hello"); // NRVO
   String first = String("first"); // direct initialization followed by copy initialization (which is elided)
   String second;
   second  =  "second";
   String third("third");
   //first = third;
   //first.move_in(third);
   first = std::move(third);
   std::cout << "I care about only these strings : " << first.c_str() << ',' << second.c_str() << "..." << std::endl;
}

