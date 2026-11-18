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

#ifndef SAMRECORD_HPP
#define SAMRECORD_HPP

#include <cerrno>
#include "debug.hh"
#include "HtsFile.hh"
#include "Locatable.hh"
#include "SamFileHeader.hh"
#include "AbstractSamRecord.hh"
namespace htspp {


class SamRecord : public Locatable, public AbstractSamRecord {
	public:
		bam1_t* b;
		std::shared_ptr<SamFileHeader> header;

		bam1_t* get() const;
		bam1_t* get();
		SamRecord(std::shared_ptr<htspp::SamFileHeader> hdr,bam1_t* b);
		SamRecord(const htspp::SamRecord& cp);
		virtual ~SamRecord();

		virtual const char* contig() const;
		virtual hts_pos_t start() const;
		virtual hts_pos_t end() const;
		virtual std::shared_ptr<SamRecord> clone() const;
		SamRecord& operator=(const SamRecord& cp);
		virtual std::ostream& print(std::ostream& out) const ;
		virtual std::string to_string() const;

		static std::shared_ptr<SamRecord> read(std::shared_ptr<htspp::HtsFile> fp,std::shared_ptr<htspp::SamFileHeader> hdr);
		};

}




#endif

