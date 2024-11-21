#ifndef HFILE_HH
#define HFILE_HH
#include <htslib/hfile.h>
#include <memory>
#include "KString.hh"
#include "utils.hh"

namespace htspp {

class HFile:public Pointer<hFILE> {
private:
        HFile(hFILE* fp);
public:
    virtual ~HFile();
    virtual void close();
    virtual bool is_open();
    virtual bool getline(KString& line);
    static std::unique_ptr<HFile> open(const char* f);
    static std::unique_ptr<HFile> open(const char* f,const char* mode);
    
};


}
#endif

