
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
/*
const std::string IoUtils::slurpFile(const char* filename) {	
	FILE * fp = fopen (filename, "rb");
	if(fp==NULL) THROW_ERROR("Cannot open script fils");
	fseek (fp, 0, SEEK_END);
	long length = ftell (fp);
	fseek (fp, 0, SEEK_SET);
	string user_expr_str;
	user_expr_str.reserve(length+1);
	if(user_expr_str==NULL) ERROR("Cannot read script file");
	if( (long)fread (user_expr_str, sizeof(char), length, fp) != length) {
		ERROR("Cannot read script file");
		}
	fclose (fp);
	string
	}*/