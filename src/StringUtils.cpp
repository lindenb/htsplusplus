#include <cctype>
#include <cstring>
#include "StringUtils.hh"

using namespace htspp;
using namespace std;

bool StringUtils::isBlank(const char* s) {
		if(s==NULL) return true;
		char* p=(char*)s;
		while(*p!=0) {
		  if(isspace(*p)) return true;
			}
		return true;
		}
bool StringUtils::startsWith(const char* s,const char* prefix) {
	return strncmp(s,prefix,strlen(prefix))==0;
	}
bool StringUtils::endsWith(const char* s,const char* suffix) {
  size_t i=strlen(s);
  size_t j=strlen(suffix);
  return i>j && strcmp(&s[i-j],suffix)==0;
	}
