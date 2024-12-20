#ifndef CIGAR_HH
#define CIGAR_HH
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include "htslib/sam.h"
#include "CigarElement.hh"
#include "CigarIterator.hh"

namespace htspp {


class Cigar {
	private:
		std::vector<CigarElement*> elements;
		Cigar();
	public:
		Cigar(bam1_t* b);
		virtual ~Cigar();
		bool empty() const;
		unsigned int size() const;
		const CigarElement* at(unsigned int idx) const;
		virtual std::ostream& print(std::ostream& out) const;
		virtual std::string to_string();
		virtual hts_pos_t reference_length() const;
		virtual hts_pos_t read_length() const;
		virtual bool is_left_clipped() const;
		virtual bool is_right_clipped() const;
		virtual bool is_clipped() const;
		static std::unique_ptr<Cigar> of(const char* s);
	};

}

#endif
