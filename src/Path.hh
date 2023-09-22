#ifndef PATH_HH
#define PATH_HH
#include <string>

class Path {
    private:
        std::string fp;
    public:   
        Path(std::string fp):fp(fp) {
            }
        Path(const Path& cp):fp(cp.fp) {
            }
        const char* c_str() const {
            return fp.c_str();
            }
    };

#endif

