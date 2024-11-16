#ifndef STRING_UTILS_HH
#define STRING_UTILS_HH

namespace htspp {

class StringUtils {
	public:
	static bool isBlank(const char* s);
  static bool startsWith(const char* s,const char* prefix);
  static bool endsWith(const char* s,const char* suffix);
	};
}

#endif
