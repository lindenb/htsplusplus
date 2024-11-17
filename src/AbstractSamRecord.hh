#ifndef ABSTRACT_SAMRECORD_HPP
#define ABSTRACT_SAMRECORD_HPP

#include <cerrno>
#include "debug.hh"
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


		virtual int num_cigar_elements() const;
		virtual bool has_cigar() const ;
		std::unique_ptr<Cigar> cigar() const;
		virtual std::vector<AlignmentBlock> alignment_blocks() const;
		virtual std::vector<AlignmentBlock>::size_type alignment_blocks(std::vector<AlignmentBlock>& array) const;
		virtual std::unique_ptr<CigarIterator> cigar_iterator();

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

