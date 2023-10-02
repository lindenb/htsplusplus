#ifndef CIGAR_HH
#define CIGAR_HH


class CigarOperator {
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

		virtual char getc() const;
		static  const CigarOperator* of(char c);
	};

class CigarElement {
	public:
		virtual const CigarOperator* op() const
		virtual hts_pos_t length() const;
	};


class Cigar {
	public:
		unsigned int size() const;
		const CigarElement* at(unsigned int) const;
	};

#endif
