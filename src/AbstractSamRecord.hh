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

#ifndef ABSTRACT_SAMRECORD_HPP
#define ABSTRACT_SAMRECORD_HPP

#include <cerrno>
#include "debug.hh"
#include "CharSequence.hh"
#include "HtsThreadPool.hh"
#include "SamFileHeader.hh"
#include "Cigar.hh"
#include "Locatable.hh"

namespace htspp {


/**

Represents the contiguous alignment of a subset of read bases to a reference sequence.

*/
struct AlignmentBlock {
		/* The first, 1-based, base in the read that is aligned to the reference reference. */
    hts_pos_t readStart;
    /* he first, 1-based, position in the reference to which the read is aligned. */
    hts_pos_t referenceStart;
    /** The number of contiguous bases aligned to the reference. */
    hts_pos_t length;
    };


class AbstractSamRecord {
	protected:
		mutable std::unique_ptr<Cigar> _cigar;
	public:
		virtual bam1_t* get() const =0;
		virtual bam1_t* get_not_null() const;
		AbstractSamRecord();
		virtual ~AbstractSamRecord();
		virtual int flags() const;
		virtual bool has_flag(int flg) const;
		virtual int tid()const;
		virtual int start0() const;
		virtual hts_pos_t start() const;
		virtual hts_pos_t end() const;
		virtual int mapq() const;
		virtual const char* read_name()  const;
		virtual bool is_unmapped() const;
        virtual bool is_mate_unmapped() const ;
         virtual bool is_paired() const;
         virtual bool is_first_of_pair() const;
         virtual bool is_second_of_pair() const;
        virtual bool is_qc_fail() const;
        virtual bool is_duplicate() const;
        virtual bool is_reverse_strand() const;
        virtual bool is_mate_reverse_strand() const;
        virtual bool is_proper_pair() const;
        virtual bool is_secondary() const;
        virtual bool is_supplementary() const;
        virtual bool is_secondary_or_supplementary() const;
        virtual bool is_ignorable() const;
        virtual bool is_ignorable(int mapq) const;


		virtual int num_cigar_elements() const;
		virtual bool has_cigar() const ;
		const Cigar* cigar() const;
		std::string cigar_string() const;
		virtual std::vector<AlignmentBlock> alignment_blocks() const;
		virtual std::vector<AlignmentBlock>::size_type alignment_blocks(std::vector<AlignmentBlock>& array) const;
		virtual std::unique_ptr<CigarIterator> cigar_iterator();

        virtual int read_length() const;
        virtual std::unique_ptr<CharSequence> read_bases() const;

        /** java htsdk functions */
        bool getReadUnmappedFlag() const;
        bool getMateUnmappedFlag() const;
        bool getReadPairedFlag() const;
        bool getFirstOfPairFlag() const;
        bool getSecondOfPairFlag() const;
        bool isSecondaryOrSupplementary() const;
        int getFlags() const;
        const char* getReadName() const;
	};



}

#endif

