#include "CharSplitter.hh"

using namespace htspp;
using namespace std;

CharSplitter CharSplitter::TAB('\t');
CharSplitter CharSplitter::COMMA(',');

bool CharSplitter::match(char c) const {
	return  delims.find(c)!=std::string::npos ;
	}

CharSplitter::CharSplitter(char delim):delims() {
	delims+=delim;
	}
CharSplitter::CharSplitter(std::string delims):delims(delims) {
	}

std::vector<std::string>::size_type CharSplitter::split(std::string s,std::vector<std::string>& v,int limit) const {
	std::string::size_type prev=0UL,i=0UL, len = s.size();
	//remove trailing
	while(len>0UL && match(s[len-1])) {
		len--;
		}
	if(limit==1) {
		v.push_back(s.substr(0,len));
		return v.size();
		}
	for(;;) {
		if(i>=len || match(s[i])) {
			v.push_back(s.substr(prev,i-prev));
			if(i>=len) break;
			prev=i+1;
			if(limit>1 && v.size()+1==(unsigned int)limit) {
				v.push_back(s.substr(prev,len-prev));
				return v.size();
				}
			}
		i++;
		}
	return v.size();
	}

std::vector<std::string>::size_type CharSplitter::split(std::string s,std::vector<std::string>& v) const {
	return split(s,v,-1);
	}

std::vector<std::string> CharSplitter::split(std::string s) const {
	std::vector<std::string>  v;
	split(s,v);
	return v;
	}
std::vector<std::string> CharSplitter::split(std::string s,int limit) const {
	std::vector<std::string>  v;
	split(s,v,limit);
	return v;
	}