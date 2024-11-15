#ifndef CIGAR_ELT_HH
#define CIGAR_ELT_HH
#include <vector>
#include <string>
#include <sstream>
#include "htslib/sam.h"
#include "CigarOperator.hh"

namespace htspp {

class CigarElement {
	private:
		char _letter;
		hts_pos_t _len;
	public:
		CigarElement(char letter, hts_pos_t len);
		virtual ~CigarElement();
		virtual const CigarOperator* op() const;
		virtual char letter() const;
		virtual hts_pos_t length() const;
		virtual std::ostream& print(std::ostream& out) const;
		virtual std::string to_string();
	};

}

#endif
