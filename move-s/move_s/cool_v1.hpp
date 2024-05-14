#pragma once
#include <string>
#include <iostream>

namespace cool {

class Shtring
{
 public:
   Shtring(char const* ptr = 0)
        : ptr_(allocate(ptr)) {
   }
   ~Shtring() {
      std::clog << "~Shtring " << *this << std::endl;
      delete ptr_;
   }
   bool empty() const { return !ptr_ || *ptr_ == '\0'; }
   char const* c_str() const { return ptr_; }
private:
    static char* allocate(char const* ptr) {
        std::size_t len;
        if (!ptr || (len = std::char_traits<char>::length(ptr)) == 0)
            return 0 /*nullptr*/;
        char* res = new char[len + 1];
        std::char_traits<char>::copy(res, ptr, len);
        res[len] = '\0';
        return res;
    }
    friend std::ostream& operator << (std::ostream& out, Shtring const& s) {
        out << &s << '{' << static_cast<void const*>(s.ptr_) << ':' << (s.empty() ? "null" : s.ptr_) << '}';
        return out;
    }
private:
    char const* ptr_ /*= nullptr*/;
};


}

