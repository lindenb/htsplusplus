#ifndef REGEX_HH
#define REGEX_HH

#include <memory>
#include "utils.hh"
#include <regex.h>

class Regex {
	private:
		regex_t regex;
		Regex();
	public:
		virtual ~Regex();
		bool matches(const char* s);
		static std::unique_ptr<Regex> compile(const char* s,int flags);
		/** parse flag for regexcmp as string, multipe separated by space, comma etc.. */
		static int parse_flags(const char* s);
	};


#endif
