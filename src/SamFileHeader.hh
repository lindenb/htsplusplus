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

#ifndef SAM_FILE_HEADER_HH
#define SAM_FILE_HEADER_HH

#include <set>
#include <memory>
#include <htslib/hts.h>
#include <htslib/sam.h>
//#include "SamSequenceDictionary.hh"
#include "utils.hh"
#include "HtsFile.hh"

namespace htspp {

class SamFileHeader:public Pointer<sam_hdr_t> {
  private:
  	//mutable std::shared_ptr<SamSequenceDictionary> _dict;
	public:
		SamFileHeader( sam_hdr_t *header);
	public:
		virtual ~SamFileHeader();
		virtual void write(htsFile* fp);
		virtual const char* tid2name(int tid) const;
		virtual std::unique_ptr<std::string> find_tag_hd(const char* tag);
		virtual std::unique_ptr<std::string> sort_order();
		virtual bool has_sort_order(const char* s);
		virtual void assert_sort_order(const char* s);
		virtual bool is_coordinate_sorted() ;
		virtual void assert_coordinate_sorted();
		virtual std::set<std::string> samples();
		virtual std::set<std::string>::size_type samples(std::set<std::string>& samples);
		//virtual std::shared_ptr<SamSequenceDictionary> dictionary() const;
		static std::shared_ptr<SamFileHeader> read(std::shared_ptr<htspp::HtsFile> fp,int flag);
	};

}

#endif

