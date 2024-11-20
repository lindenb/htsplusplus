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
		char getc() const;
		bool consumesReadBases() const;
		bool consumesReferenceBases() const;
		static  const CigarOperator* of(char c);
		static bool consumesReadBases(char c);
		static bool consumesReferenceBases(char c);
		bool is_clip() const;
		bool is_align() const;
		bool is_del() const;
		bool is_ins() const;
		bool is_indel() const;
		};

}

#endif

