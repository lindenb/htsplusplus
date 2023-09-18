
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include "debug.hh"
#include "utils.hh"

using namespace std;

bool StringUtils::isBlank(const char* s) {
	if(s==NULL) return true;
	unsigned int i=0;
	while(s[i]!=0) {
		if(!std::isspace(s[i])) return false;
		i++;
		}
	return true;
	}

const char* IoUtils::separator() {
		return "/";
		}
char IoUtils::separatorChar() {
		return '/';
		}
const bool IoUtils::assertFileExist(const char* filename) {
	FILE* fp = fopen(filename,"r");
	if(fp==NULL) {
		THROW_ERROR("Cannot open file \"" << filename << "\". "<< strerror(errno));
		}
	fclose(fp);
	return true;
	}

std::FILE* IoUtils::fopen(const char* filename, const char* mode) {
	ASSERT_NOT_NULL(filename);
	ASSERT_NOT_NULL(mode);
	std::FILE * fp = fopen (filename, mode);
	if(fp==NULL) THROW_ERROR("Cannot open \""<< filename<<"\". " << strerror(errno));
	return fp;
	}

std::string IoUtils::slurpFile(const char* filename) {
	// https://stackoverflow.com/questions/2912520
	ASSERT_NOT_NULL(filename);
	std::ifstream ifs(filename);
	if(!ifs.is_open()) THROW_ERROR("Cannot open \""<<filename<<"\". " << strerror(errno));
    std::string content(
     	(std::istreambuf_iterator<char>(ifs) ),
        (std::istreambuf_iterator<char>()    )
        );
	return content;
	}