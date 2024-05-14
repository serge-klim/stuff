#pragma once
#include <boost/core/ref.hpp>
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
      delete[] ptr_;
   }
   Shtring(Shtring const& other)
       : ptr_(allocate(other.ptr_)) {
   }
   Shtring& operator=(Shtring const& other) {
       //Shtring{other}.swap(*this);
       std::clog << "operator = & " << other;
       if (this != &other) {
           std::clog << " deleting [] " << this << '{' << static_cast<void const*>(ptr_) << ':' << (empty() ? "null" : ptr_) << '}';
           delete[] ptr_;
           ptr_ = allocate(other.ptr_);
       }
       std::clog << std::endl;
       return *this;
   }
   Shtring(boost::reference_wrapper<Shtring> other) {
       move_in(other);
   }
   Shtring& operator =(boost::reference_wrapper<Shtring> other) {
      std::clog << "operator = && " << other << ' ';
      move_in(other);
      return *this;
   }

   void swap(Shtring& other) /*noexcept*/ {
      std::swap(ptr_, other.ptr_);
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
   void move_in(Shtring& other) {
       if (this != &other) {
           std::clog << "deleting [] " << this << '{' << static_cast<void const*>(ptr_) << ':' << (empty() ? "null" : ptr_) << '}' << std::endl;
           delete[] ptr_;
           ptr_ = other.ptr_;
           other.ptr_ = 0;
       }
   }
 private:
   char const* ptr_ /*= nullptr*/;
};

} // namespace cool
