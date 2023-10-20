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
#include <set>
#include <htslib/hts.h>
#include <htslib/vcf.h>
#include "BcfReader.hh"
#include "BcfWriter.hh"
#include "ContigPos.hh"
#include "programs.hh"
#include "debug.hh"
#include "utils.hh"

using namespace std;


int main_bcfstreamuntil(int argc, char** argv) {

BcfstreamuntilArgs args;
args.parse(argc,argv);
PROGRAM_COMMON(args);
if(StringUtils::isBlank(args.position))  {
    THROW_ERROR("Bad position");
    }
ContigPos ctgpos = ContigPos::of(args.position);
bool saw_contig = false;

std::unique_ptr<BcfReader> in = BcfReader::open(optind==argc ?"-":argv[optind]);
BcfWriterFactory writer_factory;
std::unique_ptr<BcfWriter> out = writer_factory.format(args.bcf_output_format).open(args.output_filename);

out->write_header(in->header->get());

BcfRecordHeader* bcf = new BcfRecordHeader;
while(in->read2(bcf)) {
      if(!saw_contig) {
            if(ctgpos.has_contig(bcf->contig())) {
                saw_contig = true;
                if(ctgpos.position() < bcf->start()) {
                    break;
                    }
                }
            else
                {
                out->write1(bcf->get());
                }
            }
      else
            {
             if(!ctgpos.has_contig(bcf->contig()) || ctgpos.position() < bcf->start() ) {
               break;
               }
            else
                {
                out->write1(bcf->get());
                }
            }
      }
if(!saw_contig) {
    WARNING("contig " << ctgpos << " was never seen in input");    
    }
out->close();
delete bcf;
return EXIT_SUCCESS;
}
