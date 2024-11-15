#ifndef CIGAR_HH
#define CIGAR_HH
#include <vector>
#include <string>
#include <sstream>
#include "htslib/sam.h"
#include "CigarIterator.hh"

using namespace htspp;

CigarIterator::CigarIterator(bam1_t* b):i(0),n_cigar(b->core.n_cigar),cigar(bam_get_cigar(b)),letter('\0'),length(0) {
			}
			
		const CigarOperator* CigarIterator::op() {
			return CigarOperator::of(this->letter);
			}
			
		bool CigarIterator::next() {
			if(i>=n_cigar) return false;
			this->length = bam_cigar_oplen(cigar[i]);
			this->letter = bam_cigar_opchr(cigar[i]);
			++i;
			return true;
			}
