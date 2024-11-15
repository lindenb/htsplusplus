#ifndef CIGAR_OP_HH
#define CIGAR_OP_HH
#include <vector>
#include <string>
#include <sstream>
#include "htslib/sam.h"

namespace htspp {

class CigarOperator {
	private:
		bool _consumesReadBases;
		bool _consumesReferenceBases;
		char _character;
	public:
		static const CigarOperator* M;
		static const CigarOperator* I;
		static const CigarOperator* D;
		static const CigarOperator* N;
		static const CigarOperator* S;
		static const CigarOperator* H;
		static const CigarOperator* P;
		static const CigarOperator* X;
		static const CigarOperator* EQ;

		CigarOperator(bool consumesReadBases, bool consumesReferenceBases, char c);
		char getc();
		bool consumesReadBases() const;
		bool consumesReferenceBases();
		static  const CigarOperator* of(char c);
	};

}

#endif

