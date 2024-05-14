#include "cool_v4.hpp"
#include <iostream>

using String = cool::Shtring;

//void call(String);              // (1)
void call(String&);             // (2)
void call(String const&);       // (3)
//void call(String&&);            // (4)
void call(String const&&);      // (5)


int main_() {

	call("xxx");                  // (a)
	String a;
	call(a);                        // (b)
	String const b;
	call(b);                        // (c)
	call(String(b));                // (d)
	call(std::move(a));             // (e)
	call(std::move(b));             // (f)

	String make_string(const char* str);
	auto&& s = make_string("test");
	call(s);
}


void call(String) {
	std::clog << "value" << std::endl;
}
void call(String&) {
	std::clog << "&" << std::endl;
}
void call(String const&) {
	std::clog << "const &" << std::endl;
}

void call(String &&) {
	std::clog << "&&" << std::endl;
}

void call(String const&&) {
	std::clog << "const &&" << std::endl;
}
