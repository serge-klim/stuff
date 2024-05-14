#pragma once
#include <string>

namespace cool {

class Shtring
{
 public:
   Shtring() = default;
   Shtring(char const* ptr)
       : ptr_(allocate(ptr)) {
   }
   ~Shtring() noexcept {
      delete[] ptr_;
   }
   //Shtring(Shtring const& other) = delete;
   Shtring(Shtring const& other)
       : ptr_(allocate(other.ptr_)) {
   }
   //Shtring& operator=(Shtring const& other) = delete;
   Shtring& operator=(Shtring const& other) {
       //Shtring{other}.swap(*this);
       if (this != &other) {
           delete[] ptr_;
           ptr_ = allocate(other.ptr_);
       }
       return *this;
   }
   Shtring(Shtring&& other) noexcept {
      delete[] ptr_;
      ptr_ = other.ptr_;
      other.ptr_ = 0;
   }
   Shtring& operator =(Shtring&& other) noexcept {
      delete[] ptr_;
      ptr_ = other.ptr_;
      other.ptr_ = 0;
      return *this;
   }

   void swap(Shtring& other) noexcept(noexcept(std::swap(other.ptr_, other.ptr_))) {
      std::swap(ptr_, other.ptr_);
   }

   constexpr bool empty() const noexcept { return !ptr_ || *ptr_ == '\0'; }
   constexpr char const* c_str() const noexcept { return ptr_; }

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
 private:
   char const* ptr_ = nullptr;
};

} // namespace cool
