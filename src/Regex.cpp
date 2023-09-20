#include "Regex.hh"
#include "debug.hh"
#include "utils.hh"

std::unique_ptr<Regex> Regex::compile(const char* s,int flags) {
	ASSERT_NOT_NULL(s);
	Regex* r = new Regex();
	if(regcomp(&(r->regex),s,flags)!=0) {
		delete r;
		THROW_ERROR("cannot compile regular expression \"" << s << "\".");
		}
	return std::unique_ptr<Regex>(r);
	}

#define REGEXFLAG(F) else if(v[i].compare(#F)==0) {flag|=F;}

int Regex::parse_flags(const char* s) {
	ASSERT_NOT_NULL(s);
	CharSplitter splitter(", ;|\t");
	std::vector<std::string> v=splitter.split(s);
	int flag = 0;
	for(unsigned int i=0;i<v.size();++i) {
		if(StringUtils::isBlank(v[i].c_str())) continue;
		REGEXFLAG(REG_EXTENDED)
		REGEXFLAG(REG_ICASE)
		REGEXFLAG(REG_NOSUB)
		REGEXFLAG(REG_NEWLINE)
		else
			{
			THROW_ERROR("unknow regex flag " << v[i]);
			}
		}
	return flag;
	}
#undef REGEXFLAG
