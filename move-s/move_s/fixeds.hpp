#pragma once
#include <string>
#include <stdexcept>
#include <iostream>

class FixedSizeString
{
   enum{ capacity = 32}; 
 public:
   FixedSizeString(char const* ptr = nullptr) {
      std::size_t len = 0;
      if (ptr) {
         len = std::char_traits<char>::length(ptr);
         if (len >= capacity)
            throw std::length_error("length_error");
         std::char_traits<char>::copy(data_, ptr, len);
      }
      data_[len] = '\0';
   }
   ~FixedSizeString()
   {
      std::clog << "~Shtring " << this << '{' << data_ << '}' << std::endl;
   }
   bool empty() const { return *data_ == '\0'; }
   char const* c_str() const { return data_; }
 private:
   char data_[capacity];
};

