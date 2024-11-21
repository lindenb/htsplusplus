#include <cerrno>
#include "debug.hh"
#include "HFile.hh"

using namespace htspp;
using namespace std;

HFile::HFile(hFILE* f):Pointer(f) {
}


void HFile::close() {
    if(ptr!=NULL) {
         int status= ::hclose(ptr);
         if(status!=0) WARNING(ANSI_MAGENTA("Failed to close file"));
         }
    ptr=NULL; 
    }
   
bool HFile::is_open() {
    return ptr!=NULL;
    }

HFile::~HFile() {
   close();
}

std::unique_ptr<HFile> HFile::open(const char* f) {
    return open(f,"r");
    }
   
std::unique_ptr<HFile> HFile::open(const char* fname,const char* mode) {
    hFILE *f=::hopen(fname, mode);
    if(f==NULL) THROW_ERROR("cannot open " << fname << " : " << strerror(errno));
    return  std::unique_ptr<HFile>(new HFile(f));
    }
   
bool HFile::getline(KString& line) {
   if(!is_open()) return false;
   line.clear();
   return kgetline(&line.ks, (kgets_func *)hgets, get()) >= 0;
   }
