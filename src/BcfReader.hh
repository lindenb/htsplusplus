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

#ifndef BCF_READER_HH
#define BCF_READER_HH

#include "HtsFile.hh"
#include "BcfHeader.hh"
#include "BcfRecord.hh"

class BcfReader {
	public:
		std::unique_ptr<HtsFile> fp;
		std::unique_ptr<BcfHeader> header;
	private:
		BcfReader() {
			}
	public:
		virtual void close() {
			fp->close();
			}
		virtual  ~BcfReader() {
			close();
			}
		bool read1(BcfRecord* rec) {
			if(rec==NULL) return false;
			for(;;) {
			       int ret= ::bcf_read(this->fp->get(), this->header->get(), rec->get());
		       		if(rec->get()->errcode!=0) {
		            		THROW_ERROR("Error in VCF record at tid "<<rec->bcf->rid<<":"<<(rec->bcf->pos+1));
					continue;
            				}
				return ret==0;
				}
			}
		std::unique_ptr<BcfRecord> read1() {
			std::unique_ptr<BcfRecord> ret(new BcfRecord);
			if(!read1(ret.get())) {
				ret.reset();
				}
			return ret;
			}
		bool read2(BcfRecordHeader* rec) {
			if(rec==NULL) return false;
			bool b = read1(rec);
			rec->header = header->get();
			return b;
			}
		std::unique_ptr<BcfRecordHeader> read2() {
			std::unique_ptr<BcfRecordHeader> ret(new BcfRecordHeader);
			if(!read2(ret.get())) {
				ret.reset();
				}
			return ret;
			}
		static std::unique_ptr<BcfReader> open(const char* filename);
	};


#endif
