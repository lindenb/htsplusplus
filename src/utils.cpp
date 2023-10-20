
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include "debug.hh"
#include "utils.hh"

using namespace std;

hts_pos_t CoordMath::getLength(hts_pos_t start, hts_pos_t end) {
	return (end - start) + 1;
	}


bool CoordMath::overlaps(hts_pos_t start, hts_pos_t end, hts_pos_t start2,hts_pos_t end2) {
    return !(end < start2 || start > end2);
	}



CharSplitter CharSplitter::TAB('\t');
CharSplitter CharSplitter::COMMA(',');

bool StringUtils::isBlank(const char* s) {
	if(s==NULL) return true;
	unsigned int i=0;
	while(s[i]!=0) {
		if(!std::isspace(s[i])) return false;
		i++;
		}
	return true;
	}




bool StringUtils::startsWith(const char* str, const char* prefix) {
    ASSERT_NOT_NULL(str);
    ASSERT_NOT_NULL(prefix);
    return strncmp(prefix, str, strlen(prefix)) == 0;
    }

bool StringUtils::endsWith(const char* str, const char* suffix) {
    ASSERT_NOT_NULL(str);
    ASSERT_NOT_NULL(suffix);
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)   return false;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
    }



const char* IoUtils::separator() {
		return "/";
		}
char IoUtils::separatorChar() {
		return '/';
		}
const bool IoUtils::assertFileExist(const char* filename) {
	FILE* fp = std::fopen(filename,"r");
	if(fp==NULL) {
		THROW_ERROR("Cannot open file \"" << filename << "\". "<< strerror(errno));
		}
	fclose(fp);
	return true;
	}
/*
std::unique_ptr<std::ostream> IoUtils::output_or_stdout(const char* filename) {
	if(filename==NULL || strcmp(filename,"-")==0) return cout;
	ofstream out(filename,ios::out);
	if(!out.is_open()) {
		THROW_ERROR("Cannot open \""<< filename<<"\". " << strerror(errno));
		}
	return out;
	}*/

std::FILE* IoUtils::fopen(const char* filename, const char* mode) {
	ASSERT_NOT_NULL(filename);
	ASSERT_NOT_NULL(mode);
	std::FILE * fp = std::fopen (filename, mode);
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



std::vector<std::string>  IoUtils::unroll(int argc,char** argv,int optind) {
    std::vector<std::string> v;
    if(optind+1==argc && StringUtils::endsWith(argv[optind],".list")) {
        ifstream in(argv[optind]);
        if(!in.is_open()) {
             THROW_ERROR("Cannot open \""<< argv[optind] <<"\". " << strerror(errno));
            }
        std::string line;
        while(getline(in,line)) {
            if(StringUtils::isBlank(line.c_str())) continue;
            if(StringUtils::startsWith(line.c_str(),"#")) continue;
              v.push_back(line);
            }
        }
    else
        {        
        while(optind<argc) {
            v.push_back(argv[optind++]);
            }
        }
    return v;
    }
