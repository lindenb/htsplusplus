#ifndef KSTRING_HH
#define KSTRING_HH
#include <htslib/hts.h>
#include <iostream>

namespace htspp {

class KString {
	public:
		kstring_t ks;
		KString();
		KString(const char* s);
		KString(const KString& cp);
		virtual ~KString();
		virtual kstring_t* get();
		char operator[](int i) const;
		const char* c_str() const;
		KString& reserve(std::size_t n);
		std::ostream& print(std::ostream& out) const;
		
		
		std::size_t size() const;
		std::size_t length() const;
		KString& clear();
		KString& append(const char* s);
		KString& append(char c);
		KString& append(const char* s,std::size_t n);
};

}
#endif

