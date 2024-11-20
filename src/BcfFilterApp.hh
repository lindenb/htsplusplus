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

#ifndef BCF_FILTER_APP_HH
#define BCF_FILTER_APP_HH

#include "BcfHeader.hh"
#include "BcfReader.hh"
#include "BcfWriter.hh"
#include "debug.hh"


template<class T>
class BcfFilterApp: public T
	{
	public:
		BcfFilterApp() {
			}
		virtual ~BcfFilterApp() {
			}
		virtual int beforeBcf() {
			return 0;
			}
		virtual void afterBcf() {
			}

        virtual std::unique_ptr<BcfHeader> create_output_header(BcfHeader h) {
            return h->clone();            
            }

        virtual bool accept(BcfRecordHeader* rec) {
            return true;            
            }
        
        virtual int apply(BcfReader* in, BcfWriter* out) {
            BcfRecordHeader bcf;
            while(in->read2(&bcf)) {
                  if(!accept(&bcf)) continue;
                  out->write1(bcf.get());
                  }
            return 0;       
            }

		virtual int apply(const char* filein, const char* fileout) {
			if(beforeBcf()!=0) {
				WARNING("initialization failed");
				return EXIT_FAILURE;
				}
	        std::unique_ptr<BcfReader> in = BcfReader::open(filein==NULL ?"-":argv[optind]);
            BcfWriterFactory writer_factory;
            std::unique_ptr<BcfWriter> out = writer_factory.format(this->bcf_output_format).open(fileout);
            std::unique_ptr<BcfHeader> out_header = create_output_header(in->header.get());
            out->write_header(in->header->get());
            int ret = apply(in,out);
			afterBcf();
			return ret;
			}

	};


#endif

