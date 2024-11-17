#include <cassert>
#include "CigarElement.hh"

using namespace htspp;

		CigarElement::CigarElement(char letter, hts_pos_t len):_letter(letter),_len(len) {
		    assert(_len>0);
		    assert(CigarOperator::of(_letter)!=NULL);
			}
		CigarElement::~CigarElement() {
			}

		const CigarOperator* CigarElement::op() const { return CigarOperator::of(letter());}

		char CigarElement::letter() const { return _letter;}

		hts_pos_t CigarElement::length() const { return _len;}

		std::ostream& CigarElement::print(std::ostream& out) const {
			out <<length() << letter();
			return out;
			}
	    
		std::string CigarElement::to_string() {
			std::ostringstream os;
			print(os);
			return os.str();
			}
