#pragma once
#include <string>
#include <iostream>

namespace cool {

class Shtring
{
 public:
   Shtring(char const* ptr = 0)
        : ptr_(allocate(ptr)) 
   {
   }
   ~Shtring() {
      std::clog << "~Shtring " << this << '{' << static_cast<void const*>(ptr_) << ':' << (empty() ? "null" : ptr_ ) << '}' << std::endl;
      delete ptr_;
   }
   bool empty() const { return ptr_ == 0 || *ptr_ == '\0'; }
   char const* c_str() const { return ptr_; }
 private:
   static char* allocate(char const* ptr) {
      std::size_t len;
      if (ptr == 0 || (len = std::char_traits<char>::length(ptr)) == 0)
          return 0;
      char* res = new char[len + 1];
      std::char_traits<char>::copy(res, ptr, len);
      res[len] = '\0';
      return res;
   }
 private:
   char const* ptr_;
};


}

