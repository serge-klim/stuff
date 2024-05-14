#pragma once
#include <string>
#include <stdexcept>

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
   bool empty() const noexcept { return *data_ == '\0'; }
   char const* c_str() const noexcept { return data_; }
 private:
   char data_[capacity];
};

