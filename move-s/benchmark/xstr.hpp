#pragma once
#include <string>

class XString
{
 public:
   XString() = default;
   XString(char const* ptr)
       : data_{ptr} {
   }
   constexpr std::string const& data() const& noexcept { return data_; }
   std::string data()&& noexcept { return std::move(data_); }

   bool empty() const noexcept { return data_.empty(); }
   char const* c_str() const noexcept { return data_.c_str(); }
 private:
   std::string data_;
};

