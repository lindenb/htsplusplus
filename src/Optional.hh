/*
The MIT License (MIT)

Copyright (c) 2024 Pierre Lindenbaum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef OPTIONAL_HH
#define OPTIONAL_HH
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>
namespace htspp {

template<typename T>
class Optional {
private:
	T value;
	bool defined;
public:
	Optional(T value):value(value),defined(true) {
		}
  Optional():defined(false) {
		}
	Optional(const Optional& cp):value(cp.value),defined(cp.defined) {
		}
  ~Optional() {
  	}

	bool isPresent() const {
		return defined;
		}
	
	Optional<T>& operator = (const Optional<T>& cp) {
		if(this!=&cp) {
			 this->defined = cp.defined;
			 this->value = cp.value;
			 }
		return *this;
		}
	
	bool operator == (const Optional<T>& cp) const {
		if(this->defined) {
			 if(!cp.defined) return false;
			 return this->value == cp.value;
			 }
		return !cp.defined;
		}
	bool operator != (const Optional<T>& cp) const {
		return !this->operator==(cp);
		}
	operator bool() const {
  	return isPresent();
  	}
	
	T get() const {
		if(!isPresent()) throw std::runtime_error("value is not present");
		return value;
		}
	T orElse(T other) const {
		return isPresent()?value:other;
	 }
	 
		std::ostream& print(std::ostream& out) const {
	 	if(!isPresent()) {
	 			out << "<<nil>>";
	 		}
	 	else
	 		{
	 		out << value;
	 		}
	 return out;
	 }
	 
	 friend std::ostream& operator << (std::ostream& out,const Optional<T>& o) {
	 return o.print(out);
	 }
	 
	std::string to_string() const {
	 std::ostringstream out;
	 out << *(this);
	 return out.str();
	 }
	 
};

template< typename T >
Optional<T> make_optional(T value) {
	Optional<T> p(value);
	return p;
}

template< typename T >
Optional<T> make_empty_optional() {
	Optional<T> p;
	return p;
}

}

#endif

