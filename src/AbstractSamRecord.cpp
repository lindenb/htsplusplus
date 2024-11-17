#include "AbstractSamRecord.hh"

using namespace htspp;

		 bam1_t* AbstractSamRecord::get_not_null() const {
			bam1_t* b = get();
			ASSERT_NOT_NULL(b);
			return b;
			}

		AbstractSamRecord::AbstractSamRecord() {
			}
		AbstractSamRecord::~AbstractSamRecord() {
			}
		 int AbstractSamRecord::flags() const {
			return get_not_null()->core.flag;
			}
		 bool AbstractSamRecord::has_flag(int flg) const {
			return flags() & flg;
			}
		 int AbstractSamRecord::tid() const {
			return get_not_null()->core.tid;
			}
		 int AbstractSamRecord::start0() const {
			return get_not_null()->core.pos;
			}
		 hts_pos_t AbstractSamRecord::start() const {
			return start0() + 1;
			}
		 hts_pos_t AbstractSamRecord::end() const {
			return bam_endpos(get_not_null());
			}
		 const char* AbstractSamRecord::read_name() const {
			return bam_get_qname(get());
			}
		
		 bool AbstractSamRecord::is_unmapped() const{
			return has_flag(BAM_FUNMAP);
			}
         bool AbstractSamRecord::is_mate_unmapped() const{
			return has_flag(BAM_FMUNMAP);
			}
          bool AbstractSamRecord::is_paired() const {
			return has_flag(BAM_FPAIRED);
			}
          bool AbstractSamRecord::is_first_of_pair() const {
			return has_flag(BAM_FREAD1);
			}
          bool AbstractSamRecord::is_second_of_pair() const {
			return has_flag(BAM_FREAD2);
			}
         bool AbstractSamRecord::is_qc_fail() const { return has_flag(BAM_FQCFAIL);}
         bool AbstractSamRecord::is_duplicate() const { return has_flag(BAM_FDUP);}
         bool AbstractSamRecord::is_reverse_strand() const { return has_flag(BAM_FREVERSE);}
         bool AbstractSamRecord::is_mate_reverse_strand() const { return has_flag(BAM_FMREVERSE);}
         bool AbstractSamRecord::is_proper_pair() const { return has_flag(BAM_FPROPER_PAIR);}
         bool AbstractSamRecord::is_secondary() const { return has_flag(BAM_FSECONDARY);}
         bool AbstractSamRecord::is_supplementary() const { return has_flag(BAM_FSUPPLEMENTARY);}
         bool AbstractSamRecord::is_secondary_or_supplementary() const { return is_secondary() || is_supplementary() ;}


		 int AbstractSamRecord::num_cigar_elements() const {
			return get()->core.n_cigar;
			}

		 bool AbstractSamRecord::has_cigar() const {
			return num_cigar_elements() >0;
			}
		std::unique_ptr<Cigar> AbstractSamRecord::cigar() const {
			return std::unique_ptr<Cigar>(new Cigar(get()));
			}
	
std::vector<AlignmentBlock> AbstractSamRecord::alignment_blocks() const {
		 std::vector<AlignmentBlock> alignmentBlocks;
		 alignment_blocks(alignmentBlocks);
		 return alignmentBlocks;
		}	
	
std::vector<AlignmentBlock>::size_type AbstractSamRecord::alignment_blocks(std::vector<AlignmentBlock>& alignmentBlocks) const {
				CigarIterator iter(get());
		    alignmentBlocks.clear();
		    int readBase = 1;
		    int refBase = start();

		    while (iter.next()) {
		        switch (iter.letter()) {
		            case 'H':
		                break; // ignore hard clips
		            case 'P':
		                break; // ignore pads
		            case 'S':
		                readBase += iter.length();
		                break; // soft clip read bases
		            case 'N':
		                refBase += iter.length();
		                break;  // reference skip
		            case 'D':
		                refBase += iter.length();
		                break;
		            case 'I':
		                readBase += iter.length();
		                break;
		            case 'M':
		            case '=':
		            case 'X': {
		                int n = iter.length();
		                AlignmentBlock ab;
		                ab.readStart = readBase;
										ab.referenceStart = refBase;
										ab.length = n;
										alignmentBlocks.push_back(ab);
		                readBase += n;
		                refBase += n;
		                break;
		                }
		            default:
		                THROW_ERROR("cigar not handled");
		                break;
		        	}
		    	}
        	return alignmentBlocks.size();
        	}


std::unique_ptr<CigarIterator> AbstractSamRecord::cigar_iterator() {
	return std::unique_ptr<CigarIterator>(new CigarIterator(get()));
}



        /** java htsdk functions */
        bool AbstractSamRecord::getReadUnmappedFlag() const {return is_unmapped();}
        bool AbstractSamRecord::getMateUnmappedFlag() const {return is_mate_unmapped();}
        bool AbstractSamRecord::getReadPairedFlag() const { return is_paired();}
        bool AbstractSamRecord::getFirstOfPairFlag() const { return is_first_of_pair();}
        bool AbstractSamRecord::getSecondOfPairFlag() const { return is_second_of_pair();}
        bool AbstractSamRecord::isSecondaryOrSupplementary() const { return is_secondary_or_supplementary();}
        int AbstractSamRecord::getFlags() const { return flags();}
        const char* AbstractSamRecord::getReadName() const { return read_name();}
