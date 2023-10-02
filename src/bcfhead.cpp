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

int main_bcfhead(int argc,char** argv) {
BcfheadArgs args;
args.parse(argc,argv);
PROGRAM_COMMON(args);
if(args.count< 0L)  {
    THROW_ERROR("Bad count '" << args.count << " <=0.");
    }

std::unique_ptr<BcfReader> in = BcfReader::open(optind==argc ?"-":argv[optind]);
BcfWriterFactory writer_factory;
std::unique_ptr<BcfWriter> out = writer_factory.format(args.bcf_output_format).open(args.output_filename);

out->write_header(in->header->get());

BcfRecordHeader* bcf = new BcfRecordHeader;
int64_t n_read=0;
while(in->read2(bcf) && n_read< args.count) {
      out->write1(bcf->get());
      ++n_read;     
      }
out->close();
delete bcf;
return EXIT_SUCCESS;
}
