
#include <cerrno>
#include <cstdio>
#include <cstring>
#include "debug.hh"
#include "utils.hh"

using namespace std;

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