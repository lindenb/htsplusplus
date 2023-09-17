/*
The MIT License (MIT)

Copyright (c) 2020 Pierre Lindenbaum

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
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <htslib/hts.h>
#include <htslib/vcf.h>
#include "BcfReader.hh"
#include "BcfWriter.hh"
#include "programs.hh"
#include "debug.hh"
using namespace std;

int main_bcfprune(int argc,char** argv) {
BcfpruneArgs args;
args.parse(argc,argv);
PROGRAM_COMMON(args);
if(args.distance<=0)  {
    THROW_ERROR("Bad distance '" << args.distance << " <=0.");
    }
if(!(optind==argc || optind<argc)) {
    THROW_ERROR("Illegal number of arguments");
    }
std::unique_ptr<BcfReader> in = BcfReader::open(optind==argc ?"-":argv[optind]);
BcfWriterFactory writer_factory;
std::unique_ptr<BcfWriter> out = writer_factory.format(args.bcf_output_format).open(args.output_filename);

out->write_header(in->header->get());

BcfRecordHeader* bcf = new BcfRecordHeader;
hts_pos_t prev_end = -1;
hts_pos_t prev_start = -1;
int32_t prev_tid=-1;
int ret=0;
while(in->read2(bcf)) {
       if(prev_tid>=0 && (prev_tid> bcf->get()->rid || (prev_tid==bcf->get()->rid && bcf->get()->pos < prev_start))) {
            THROW_ERROR("Data are not ordered (tid "<<prev_tid<<":" << prev_start+1 <<") vs (tid:" << bcf->get()->rid << ":" << (bcf->get()->pos+1)<< ")");
            }
       if(bcf->tid()!=prev_tid || (bcf->start() - prev_end) >= args.distance ) {
            out->write1(bcf->get());
            prev_tid = bcf->tid();
            prev_start = bcf->start();
            prev_end = bcf->end();
            }
      }
out->close();
delete bcf;
if(ret<-1) {
    THROW_ERROR("IO error in input VCF.\n");
    }
return EXIT_SUCCESS;
}
