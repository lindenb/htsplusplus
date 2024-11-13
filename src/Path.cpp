#include "Path.hh"

Path::Path(std::string fp):fp(fp) {
            }
Path::Path(const Path& cp):fp(cp.fp) {
            }
const char* Path::c_str() const {
    return fp.c_str();
    }
 