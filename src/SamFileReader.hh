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

#ifndef SAMFILE_HPP
#define SAMFILE_HPP

#include <cerrno>
#include "debug.hh"
#include "HtsThreadPool.hh"
#include "SamRecord.hh"
#include "SamFileHeader.hh"
#include "SamRecordIterator.hh"
#include "Coverage.hh"
#include "HtsFile.hh"
#include "HtsIdx.hh"

namespace htspp {


class SamFileReader {
	public:
		typedef int (*callback1_t)(SamRecord*,void* data);
		std::unique_ptr<HtsFile> fp;
		std::unique_ptr<SamFileHeader> header;
		//associated index
		std::unique_ptr<HtsIndex> idx;
		SamFileReader();
		virtual ~SamFileReader();
		virtual bool has_index();
		virtual bool read1(bam1_t* b);
		virtual bool read2(SamRecord* rec);
		virtual std::unique_ptr<SamRecord> read2();
		virtual std::unique_ptr<SamRecordIterator> query_by_loc(const Locatable* loc);
		virtual std::unique_ptr<SamRecordIterator> query(const char* reg);
		virtual std::unique_ptr<SamRecordIterator> iterator();
		virtual std::shared_ptr<SamSequenceDictionary> dictionary() const;
	};

}

#endif

