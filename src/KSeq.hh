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

#ifndef KSEQ_HH
#define KESQ_HH

#include <htslib/hts.h>
#include <string>
#include "signal.h"
#include <zlib.h>
#include "htslib/kseq.h"
#include "debug.hh"

KSEQ_INIT(gzFile, gzread)


class KSeqReader {
	public:
		kseq_t* ks1;
		KSeqReader(gzFile fp1):ks1(NULL) {
			ks1 = ::kseq_init(fp1);
			if(ks1==NULL) {
				THROW_ERROR("Cannot initialize reader");
				}
			}
		bool next() {
			return ::kseq_read(ks1) >= 0;
			}

		const char* name() {
			return ks1->name.s;
			}
		const char* seq() {
			return ks1->seq.s;
			}
		const char* qual() {
			return ks1->qual.s;
			}
	private:
		void write(gzFile out,kstring_t& t) {
			::gzwrite(out,(void*)(t.s),t.l);
			}
	public:

		void writeFastq(gzFile out) {
			::gzputc(out,'@');
			write(out,ks1->name);
			::gzputc(out,'\n');
			write(out,ks1->seq);
			::gzputc(out,'\n');
			::gzputc(out,'+');
			::gzputc(out,'\n');
			write(out,ks1->qual);
			if(::gzputc(out,'\n')==-1) {
				THROW_ERROR("I/O error");
				}
			}


		virtual void close() {
			if(ks1!=NULL) ::kseq_destroy(ks1);
			ks1 = NULL;
			}

		virtual ~KSeqReader() {
			close();
			}
	};

#endif

