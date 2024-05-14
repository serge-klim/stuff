```  
I have no interest in idle dreams, I'm a practical pig.  
                                  Napoleon, Animal Farm, George Orwell   
```  
so let's look into it from practical side, what and why exactly we are optimizing?


below based on what I've seen in the codebase during reviews.

Which expressions produce copy, if any:
```   
   using String = std::string;

   String make_string(const char* str) {
      String res(str);
      return res;
   }
   
   String hello = make_string("hello"); // (1)
   String first = String("first");      // (2)
   String second;                       // (3)
   second = "second";                   // (4)
   String third("third");               // (5)
   first = third;                       // (6)
```


```
   String forth = third                 // (7)
```

few questions I believe one should ask yourself about code: 
* would it compile?
  - [well-formatted](https://eel.is/c++draft/defns.well.formed)
* would it run?
  - [undefined behavior](https://eel.is/c++draft/intro.defs#defns.undefined)
* would it produce expected result?
  - [implementation-defined behavior](https://eel.is/c++draft/intro.defs#defns.impl.defined)
  - [unspecified behavior](https://eel.is/c++draft/intro.defs#defns.unspecified)

```























   String hello = make_string("hello"); // (1) NRVO
   String first = String("first");      // (2) direct initialization followed by copy initialization (which is elided)
   String second;                       // (3) Default constructor
   second = "second";                   // (4) assignment 
   String third("third");               // (5) direct initialization
   first = third;                       // (6) assignment 
   String forth = third                 // (7) copy constructor
```
[Note: msvc [/Zc:nrvo (Control optional NRVO)](https://learn.microsoft.com/en-us/cpp/build/reference/zc-nrvo?view=msvc-170)]

[You don't pay for what you don't use](https://en.cppreference.com/w/cpp/language/Zero-overhead_principle) - sort of. 


#### guinea pig - implementation requirement:
* above should compile
* only these are required:
  - `bool empty()`
  - `char const* c_str()`
 
 ```
 /* attached code*/







 ```
   
[The rule of three:](https://en.cppreference.com/w/cpp/language/rule_of_three)
If a class requires a user-defined destructor, a user-defined copy constructor, or a user-defined copy assignment operator, it almost certainly requires all three. 

Note: Consider this class: 
```
class XString
{
 public:
   XString(){}
   XString(char const* ptr): data_{ptr}{}
   bool empty() const { return data_.empty(); }
   char const* c_str() const { return data_.c_str(); }
 private:
   std::string data_;
};
```
Same three questions?

#### How can we improve it in terms of C++03 first?  

`void move_in(Shtring& other)` - and that's exactly what move assignment operator does!

#### So why do we need move semantic?

```
  template<typename T> using Vector = std::vector<T>;
  auto v = Vector<String>{};
  v.push_back(String{"new value"});  //??????????????
```

How to solve this?
How about using something similar to `std::reference_wrapper`? 
```
template<typename T>
using movable_ref = std::renamed_reference_wrapper<T>; // ???????????????
```
As an exercise, one can extend Shtring with that and see what would happen, bear in mind you have to use C++98(03) compiler!

in other word object should be marked somehow to distinguish copying from moving. Technically it could be achieved by:
  - library feature
  - or language feature  
 
c++ committee choose second option and this feature changed a language quite a bit :)  

let's see what changed:  
####  && - rvalue reference declarator.
[Overload resolution](https://eel.is/c++draft/over.match)
before introducing &&: 
```
    void call(String);                        // (1)
    void call(String&);                       // (2)
    void call(String const&);                 // (3)
                                              
    call("xxx");                              // (a)
    String a;
    call(a);                                  // (b)
    String const b;
    call(b);                                  // (c)
    call(String(b));                          // (d)
``` 
```






















    call("xxx");                    // (1) or (3)
    String a;
    call(a);                        // (1) or (2) also (3) if (1) or (2) are not declared
    String const b;
    call(b);                        // (1) or (3)
    call(String(b));                // (1) or (3)
```
basically copying or referencing. 

`#include <utility>`  
`template<class T> constexpr remove_reference_t<T>&& move(T&& t) noexcept;`  
[Returns: static_cast<remove_reference_t<T>&&>(t)](https://eel.is/c++draft/utilities#forward-10)

now that we have the way to mark objects we want to reuse, back to original problem:
```
  auto s = String{"some value"}
  auto v = Vector<String>{};
  v.push_back(std::move(v));
```

ok would this compile now:
```
template<typename T>
struct Vector{
  void push_back(T&& val){
     other().push_back(val);
  }
  Vector& other();
};
```

consider following code:  
```
auto&& str = make_string("some value");
v.push_back(str);    // (1)
v.push_back(str);    // (2)



















```  

#### what's happen to the object we moved from?
still valid object: 
 * destructor will be called
 * value can be assigned (not strictly required)  
 
[C.64: A move operation should move and leave its source in a valid state](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-move-semantic)
std library guaranties for classes:   
[Unless otherwise specified, such moved-from objects shall be placed in a valid but unspecified state](https://eel.is/c++draft/lib.types.movedfrom)  

[C.65: Make move assignment safe for self-assignment](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c65-make-move-assignment-safe-for-self-assignment)  



since new declarator has been added overloading rules updated as well:
```
    void call(String);              // (1)
    void call(String&);             // (2)
    void call(String const&);       // (3)
    void call(String &&);           // (4)
    void call(String const&&);      // (5)

    call("xxx");                    // (a)
    String a;
    call(a);                        // (b)
    String const b;
    call(b);                        // (c)
    call(String(b));                // (d)
    call(std::move(a));             // (e)
    call(std::move(b));             // (f)




















    call("xxx");                    // (1) or (4), also (5) and (3)
    String a;
    call(a);                        // (1) or (2), also (3)
    String const b;
    call(b);                        // (1) or (3)
    call(String(b));                // (1) or (4), also (5) and (3)
    call(std::move(a));             // (1) or (4), also (5) and (3)
    call(std::move(b));             // (1) or (5), also (3)
 ```

 why is it relevant?
 same rules apply when choosing in between move and copy operations.
 in other words if class doesn't have move constructor/assignment operator, copy constructor or copy operator will be used with rvalue reference.





**Expressions** are categorized according to the taxonomy...     
[Value category](https://eel.is/c++draft/basic.lval)
[[Note 2: Historically, lvalues and rvalues were so-called because they could appear on the left- and right-hand side of an assignment (although this is no longer generally true); glvalues are “generalized” lvalues, prvalues are "pure" rvalues, and xvalues are "eXpiring" lvalues.
Despite their names, these terms apply to **expressions**, not values.
— end note]](https://eel.is/c++draft/basic.lval#3)

#### Return type 

given a,b and c are they are same type or appropriate conversion(s) are exists and + operator defined
```
    a + b = c;   
























    int a = 5, b = 15, c = 7;
    a + b = c;

    struct A{};
    A operator + (A const&, A const&);

    A aa, ab, ac;
    aa + ab = ac;
```
* no more RVO since C++17:)
[Temporary materialization conversion](https://eel.is/c++draft/conv.rval)
[The materialization of a temporary object is generally delayed as long as possible in order to avoid creating unnecessary temporary objects.](https://eel.is/c++draft/class.temporary#2)
```
   String make_string(const char* str) {
      return /*String*/{str};
   }
```     

* * NRVO
[Copy/move elision ](https://eel.is/c++draft/class.copy.elision)
```
   String make_string(const char* str) {
      String res(str);
      return res;
   }
``` 

#### Returning by value should look like a reasonable thing now, shouldn't it?
  + are you really need that named temporary? 
  [F.45: Don’t return a T&&](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f45-dont-return-a-t)

```
  tempplate<typename F, typename S>
  /*auto*/std::pair<F,S> stuff(std::pair<F,S> x){
     return std::move(x);
  }
```
```
  tempplate<typename F, typename S>
  /*auto*/ S stuff(std::pair<F,S> x){
     return std::move(x.second);
  }
```

#### Private parts? more overloading.
```
  class XString{
     constexpr std::string const& data() const& noexcept { return data_; }
     std::string data()&& noexcept { return std::move(data_); }
  private:
     std::string data_;
  };

  auto str = XString{} 
  auto data1 = str.data();
  auto data2 = std::move(str).data();
  return std::move(str).data();
```


#### What about input parameters?
`void call(A val);`  
vs  
```
void call(A const& val);
void call(A&& val);
```


#### Refine initial implementation
[If the definition of a class X does not explicitly declare a move constructor, one will be implicitly declared as defaulted if and only if](https://eel.is/c++draft/class.copy#9)
  - (9.1) X does not have a user-declared copy constructor,
  - (9.2) X does not have a user-declared copy assignment operator,
  - (9.3) X does not have a user-declared move assignment operator, and
  - (9.4) X does not have a user-declared destructor. 
  [ Note: When the move constructor is not implicitly declared or explicitly supplied, expressions that otherwise would have invoked the move constructor may instead invoke a copy constructor.  — end note ]

[If the definition of a class X does not explicitly declare a move assignment operator, one will be implicitly declared as defaulted if and only if](https://eel.is/c++draft/class.copy#20)
  - (20.1) X does not have a user-declared copy constructor,
  - (20.2) X does not have a user-declared move constructor,
  - (20.3) X does not have a user-declared copy assignment operator, and
  - (20.4) X does not have a user-declared destructor. 

### Summary:    
  * Please let compiler do its job - it is good at it;
  * Less code - most likely less bugs/better performance;
  * Make it simple, measure - only optimize what's needs to be optimized;

#### References:   
  https://www.cppmove.com/  
  https://www.youtube.com/watch?v=Bt3zcJZIalk  
  https://www.youtube.com/watch?v=TFMKjL38xAI  
  https://www.youtube.com/watch?v=IZbL-RGr_mk  
  https://www.youtube.com/watch?v=kPR8h4-qZdk 
  

would this compile:
```
template<typename T>
struct Vector{
  void push_back(T&& val){
     other().push_back(val);
  }
  void push_back(auto&& val);

  Vector& other();
};
```