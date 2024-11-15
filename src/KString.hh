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
		char operator[](int i);
		char* c_str();
		std::ostream& print(std::ostream& out) const;
	};

}

#endif

