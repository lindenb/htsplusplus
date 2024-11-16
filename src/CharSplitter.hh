#ifndef CHARSPLITTER_HH
#define CHARSPLITTER_HH
#include <string>
#include <vector>

namespace htspp {

class CharSplitter {
	private:
		std::string delims;
	protected:
		virtual bool match(char c) const;
	public:
		CharSplitter(char delim);
		CharSplitter(std::string delims);
		std::vector<std::string>::size_type split(std::string s,std::vector<std::string>& v,int limit) const;
		std::vector<std::string>::size_type split(std::string s,std::vector<std::string>& v) const;
		std::vector<std::string> split(std::string s,int limit) const;
		std::vector<std::string> split(std::string s) const;
		static CharSplitter TAB;
		static CharSplitter COMMA;
	};
}

#endif
