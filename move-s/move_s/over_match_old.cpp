#include "cool_v4.hpp"
#include <iostream>

using String = cool::Shtring;

//void call(String);                // (1)
void call(String&);                 // (2)
void call(String const&);           // (3)


int main() {
	call("xxx");                    // (a)
	String a;
	//call(a);                      // (b)
	String const b;
	call(b);                        // (c)
	call(String(b));                // (d)
	return 0;
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
