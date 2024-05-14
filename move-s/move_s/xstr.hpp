#pragma once
#include <string>

class XString
{
 public:
   XString(char const* ptr = "")
       : data_{ptr} {
   }
   ~XString() {
      std::clog << "~XString " << this << '{' << data_ << '}' << std::endl;
   }
   bool empty() const { return data_.empty(); }
   char const* c_str() const { return data_.c_str(); }
 private:
   std::string data_;
};

