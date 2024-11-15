#ifndef CIGAR_ITER_HH
#define CIGAR_ITER_HH
#include <vector>
#include <string>
#include <sstream>
#include "htslib/sam.h"
#include "CigarOperator.hh"

namespace htspp {

class CigarIterator {
	private:
		int i;
		int n_cigar;
	 	uint32_t *cigar;
		char _letter;
		int _length;
	public:
		char letter();
		int length();
		CigarIterator(bam1_t* b);
		const CigarOperator* op();
		bool next();
	};

}

#endif
