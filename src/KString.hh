#ifndef KSTRING_HH
#define KSTRING_HH
#include <htslib/hts.h>
#include "CharSequence.hh"

namespace htspp {

class KString : public CharSequence {
	public:
		kstring_t ks;
		KString();
		KString(const char* s);
		KString(const KString& cp);
		virtual ~KString();
		virtual kstring_t* get();
		virtual char at(int i) const;
		virtual char& operator[](int i);
		virtual const char* c_str() const;
		virtual KString& reserve(std::size_t n);
		virtual std::ostream& print(std::ostream& out) const;
		
		

		virtual std::size_t length() const;
		virtual KString& clear();
		virtual KString& append(const char* s);
		virtual KString& append(char c);
		virtual KString& append(const char* s,std::size_t n);
		
};

std::ostream& operator<<(std::ostream& out, const KString& cp);

}
#endif

