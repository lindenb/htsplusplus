#include <vector>
#include <string>
#include <sstream>
#include "htslib/sam.h"
#include "CigarIterator.hh"

using namespace htspp;

CigarIterator::CigarIterator(bam1_t* b):i(-1),n_cigar(b->core.n_cigar),cigar(bam_get_cigar(b)),_letter('\0'),_length(0) {
			}

const CigarOperator* CigarIterator::op() {
			return CigarOperator::of(letter());
			}

bool CigarIterator::next() {
			i++;
			if(i>=n_cigar) {
    	    		this->_length = 0;
    			    this->_letter = '\0';
			        return false;
			        }
			this->_length = bam_cigar_oplen(cigar[i]);
			this->_letter = bam_cigar_opchr(cigar[i]);
			return true;
			}

char CigarIterator::letter() {
    return _letter;
    }
int CigarIterator::length() {
    return _length;
    }
