#ifndef PATH_HH
#define PATH_HH
#include <string>

class Path {
    private:
        std::string fp;
    public:   
        Path(std::string fp);
        Path(const Path& cp);
        const char* c_str() const;
    };

#endif

