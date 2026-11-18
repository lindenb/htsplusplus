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
#include "debug.hh"
#include "HtsFile.hh"
#include "SamRecord.hh"
#include "ProgramOnePassBam.hh"
#include "SamFileHeader.hh"

using namespace std;
using namespace htspp;

ProgramOnePassBam::ProgramOnePassBam(const char* name,const char* description):Program(name,description),
        reference(new OptionValue<string>('T',"reference","path to indexed fasta reference","")),
        file_out(new OptionValue<string>('o',"out","output file or standout","-"))
        {
    getopt.add(reference);
    getopt.add(file_out);
    }

ProgramOnePassBam::~ProgramOnePassBam() {
    } 


int ProgramOnePassBam:: main(int argc,char** argv) {
    		cerr << argc << endl;

   const char* input= oneFileOrNull(argc,argv);
   shared_ptr<HtsFile> fp= HtsFile::open(input==NULL?"-":input,"rb",0);
   if(!fp || !fp->is_open()) {
        THROW_ERROR("Cannot open " << (input==NULL?"-":input));
        return -1;
        }
  shared_ptr<SamFileHeader> header_in = SamFileHeader::read(fp,0);
  if(!header_in) {
        THROW_ERROR("Cannot read header from " << (input==NULL?"-":input));
        return -1;
        }
    for(;;) {
        shared_ptr<SamRecord> rec= SamRecord::read(fp,header_in);
        if(!rec) break;
        rec->print(std::cerr);
        }
    cerr << "done." << endl;

  return 0;
  }