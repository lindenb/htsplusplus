#ifndef CIGAR_HH
#define CIGAR_HH
#include <vector>
#include <string>
#include <sstream>
#include "htslib/sam.h"


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

		CigarOperator(bool consumesReadBases, bool consumesReferenceBases, char c):_consumesReadBases(consumesReadBases),_consumesReferenceBases(consumesReferenceBases),_character(c){
			}

		char getc() const { return _character;}
		bool consumesReadBases() const { return _consumesReadBases;}
		bool consumesReferenceBases() const { return _consumesReferenceBases;}
		static  const CigarOperator* of(char c);
	};

/*




*/

class CigarIterator {
	private:
		int i;
		int n_cigar;
	 	uint32_t *cigar;
	public:
		char letter;
		int length;
		
		
		CigarIterator(bam1_t* b):i(0),n_cigar(b->core.n_cigar),cigar(bam_get_cigar(b)),letter('\0'),length(0) {
			}
			
		const CigarOperator* op() {
			return CigarOperator::of(this->letter);
			}
			
		bool next() {
			if(i>=n_cigar) return false;
			this->length = bam_cigar_oplen(cigar[i]);
			this->letter = bam_cigar_opchr(cigar[i]);
			++i;
			return true;
			}
	};

class CigarElement {
	private:
		char _letter;
		hts_pos_t _len;
	public:
		CigarElement(char letter, hts_pos_t len):_letter(letter),_len(len) {
			}
		virtual ~CigarElement() {
			}
		virtual const CigarOperator* op() const { return CigarOperator::of(_letter);}
		virtual char letter() const { return _letter;}
		virtual hts_pos_t length() const { return _len;}
		virtual std::ostream& print(std::ostream& out) const {
			out << letter() << length();
			return out;
			}
		virtual std::string to_string() {
			std::ostringstream os;
			print(os);
			return os.str();
			}
	};


class Cigar {
	private:
		std::vector<CigarElement*> elements;
	public:
		Cigar(bam1_t* b) {
			CigarIterator iter(b);
			elements.reserve(b->core.n_cigar);
			while(iter.next()) {
				elements.push_back(new CigarElement(iter.letter, iter.length));
				}
			}
		virtual ~Cigar() {
			for(unsigned int i=0;i< elements.size();i++) {
				delete elements[i];
				}
			}
		bool empty() const {
			return elements.empty();
			}
		unsigned int size() const {
			return elements.size();
			}
		const CigarElement* at(unsigned int idx) const {
			return elements.at(idx);
			}
		virtual std::ostream& print(std::ostream& out) const {
			for(unsigned int i=0; i< size();i++) {
				at(i)->print(out);
				}
			return out;
			}
		virtual std::string to_string() {
			std::ostringstream os;
			print(os);
			return os.str();
			}
	};


#endif
