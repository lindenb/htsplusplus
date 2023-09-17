#include "Regex.hh"
#include "debug.hh"

std::unique_ptr<Regex> Regex::compile(const char* s,int flags) {
	ASSERT_NOT_NULL(s);
	Regex* r = new Regex();
	if(regcomp(&(r->regex),s,flags)!=0) {
		delete r;
		THROW_ERROR("cannot compile regular expression \"" << s << "\".");
		}
	return std::unique_ptr<Regex>(r);
	}

