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
