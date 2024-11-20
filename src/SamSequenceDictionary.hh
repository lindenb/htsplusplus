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

#ifndef SAM_SEQUENCE_DICT_HH
#define SAM_SEQUENCE_DICT_HH
#include <cstring>
#include <string>

#include <htslib/faidx.h>
#include <htslib/sam.h>
#include "SamSequenceRecord.hh"
#include "utils.hh"
#undef null
#include <htslib/vcf.h>

namespace htspp {

class SamSequenceDictionary {

	public:
		static const unsigned int npos = (unsigned int)-1UL;;
		SamSequenceDictionary();
		virtual ~SamSequenceDictionary();
		virtual int32_t nseq() const = 0;
		virtual const SamSequenceRecord* at(int32_t) const = 0;
		virtual const SamSequenceRecord* getSequence(const char* str) const = 0;
		virtual long genome_length() const = 0;
		virtual bool equals(const SamSequenceDictionary& cp) const;
		virtual bool operator==(const SamSequenceDictionary& cp) const;
		virtual std::unique_ptr<std::string> resolve_contig(const char* s) const;
		virtual std::unique_ptr<Locatable> parseInterval(const char* s) const;
		
		static std::unique_ptr<SamSequenceDictionary> of_sam_header(sam_hdr_t *header);
		static std::unique_ptr<SamSequenceDictionary> of_bcf_header(bcf_hdr_t *header);
		static std::unique_ptr<SamSequenceDictionary> of_faidx(faidx_t *fai);
	};

}

#endif

