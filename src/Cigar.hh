/*
The MIT License (MIT)

Copyright (c) 2024 Pierre Lindenbaum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef CIGAR_HH
#define CIGAR_HH
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include "CigarElement.hh"
#include "CigarIterator.hh"

namespace htspp {

/**
 * Implementation of a cigar string
 */
class Cigar {
	protected:
		std::vector<CigarElement*> elements;
	public:
		Cigar();
		Cigar(bam1_t*);
		virtual ~Cigar();
		bool empty() const;
		virtual unsigned int size() const;
		virtual const CigarElement* at(unsigned int idx) const;
		virtual std::ostream& print(std::ostream& out) const;
		virtual std::string to_string();
		virtual hts_pos_t reference_length() const;
		virtual hts_pos_t read_length() const;
		virtual bool is_left_clipped() const;
		virtual bool is_right_clipped() const;
		virtual bool is_clipped() const;
		static std::shared_ptr<Cigar> of(const char* s);
	};

}

#endif
