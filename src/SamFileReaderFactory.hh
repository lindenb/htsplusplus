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

#ifndef SAMFILE_READER_FACTORY_HPP
#define SAMFILE_READER_FACTORY_HPP

#include <cerrno>
#include "debug.hh"
#include "HtsThreadPool.hh"
#include "HtsIdx.hh"
#include "SamRecord.hh"
#include "SamFileReader.hh"
#include "Coverage.hh"

namespace htspp {

class SamFileReaderFactory {
	private:
		HtsThreadPool* threadPool;
		std::string _reference;
	protected:
	  virtual SamFileReader* createNewSamFileReader();
	public:
		SamFileReaderFactory();
		virtual ~SamFileReaderFactory();
		virtual SamFileReaderFactory& threads(HtsThreadPool* threadPool);
		virtual SamFileReaderFactory& reference(const char* fasta);
		virtual std::unique_ptr<SamFileReader> open();//stdin
		virtual std::unique_ptr<SamFileReader> open(const char* filename);
		virtual std::unique_ptr<SamFileReader> open(const char* filename,bool require_index);
		virtual std::unique_ptr<SamFileReader> open(const char* filename,const char* index_or_NULL,bool require_index);
	};

}


#endif

