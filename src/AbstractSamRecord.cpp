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

#include "AbstractSamRecord.hh"

using namespace htspp;
using namespace std;

class Cigar2: public Cigar {
    private:
        vector<CigarElement*> elements;
    public:
       Cigar2(const AbstractSamRecord* owner) {
          assert(owner->get()!=NULL);
          CigarIterator iter(owner->get());
          elements.reserve(owner->get()->core.n_cigar);
          while(iter.next()) {
                  elements.push_back(new CigarElement(iter.letter(), iter.length()));
                  }
          assert(elements.size()==owner->get()->core.n_cigar);
        }
       virtual ~Cigar2() {
        for(auto ce: elements) delete ce;
        }

       virtual unsigned int size() const { return  elements.size();}
       virtual const CigarElement* at(unsigned int idx) const {
            return elements.at(idx);
            }

    };


		 bam1_t* AbstractSamRecord::get_not_null() const {
			bam1_t* b = get();
			ASSERT_NOT_NULL(b);
			return b;
			}

		AbstractSamRecord::AbstractSamRecord() {
			}
		AbstractSamRecord::~AbstractSamRecord() {
			}
			
		 int AbstractSamRecord::mapq() const {
			return get_not_null()->core.qual;
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

	 bool AbstractSamRecord::is_ignorable() const {
		return is_unmapped() || is_duplicate() || is_secondary_or_supplementary() || is_qc_fail();
		}
	 bool AbstractSamRecord::is_ignorable(int mapQ) const {
		return is_ignorable() || mapq() < mapQ;
		}

		 int AbstractSamRecord::num_cigar_elements() const {
			return get()->core.n_cigar;
			}

		 bool AbstractSamRecord::has_cigar() const {
			return num_cigar_elements() >0;
			}
		const Cigar* AbstractSamRecord::cigar() const {
		    if(!_cigar) {
		        _cigar.reset(new Cigar2(this));
		        }
			return _cigar.get();
			}
	
		std::string AbstractSamRecord::cigar_string() const {
			if(!has_cigar()) return "*";
			Cigar2 c(this);
		  return c.to_string();
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
        
 /* defined in htslib/sam_internal.h */
 extern void nibble2base(uint8_t *nib, char *seq, int len);
 
class ReadSeq: public CharSequence {
    private:
        uint8_t *s;
        int len;
    public:
       ReadSeq(uint8_t *s,int len):s(s),len(len) {
        }
       virtual ~ReadSeq() {
        };
        
       virtual char at(int pos) const {
         return seq_nt16_str[bam_seqi(s, pos)];
         }
       virtual std::size_t length() const {
        return len;
        }
    };
        
int AbstractSamRecord::read_length() const {
    return get()->core.l_qseq;
    }
    
std::unique_ptr<CharSequence> AbstractSamRecord::read_bases() const {
    std::unique_ptr<CharSequence> p;
    if(read_length()==0) return p;
    p.reset(new ReadSeq( bam_get_seq(get()),read_length())) ;
    return p;
    }
   
