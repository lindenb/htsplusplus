/*
The MIT License (MIT)

Copyright (c) 2024 Pierre Lindenbaum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
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
