#ifndef KSTRING_HH
#define KSTRING_HH
#include <htslib/hts.h>
class KString {
	public:
		kstring_t ks;
		KString();
		KString(const char* s);
		KString(const KString& cp);
		virtual ~KString();
		char* c_str();
	};

#endif

