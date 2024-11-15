#include "CigarElement.hh"

using namespace htspp;

		CigarElement::CigarElement(char letter, hts_pos_t len):_letter(letter),_len(len) {
			}
		CigarElement::~CigarElement() {
			}
		const CigarOperator* op() const { return CigarOperator::of(_letter);}
		char CigarElement::letter() const { return _letter;}
		hts_pos_t CigarElement::length() const { return _len;}
		std::ostream& CigarElement::print(std::ostream& out) const {
			out << letter() << length();
			return out;
			}
		std::string CigarElement::to_string() {
			std::ostringstream os;
			print(os);
			return os.str();
			}
