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

#include <vector>
#include <string>
#include <sstream>
#include "htslib/sam.h"
#include "CigarIterator.hh"

using namespace htspp;

CigarIterator::CigarIterator(bam1_t* b):i(-1),n_cigar(b->core.n_cigar),cigar(bam_get_cigar(b)),_letter('\0'),_length(0) {
			}

CigarIterator::~CigarIterator() {
}

const CigarOperator* CigarIterator::op() {
			return CigarOperator::of(letter());
			}

bool CigarIterator::next() {
			i++;
			if(i>=n_cigar) {
    	    		this->_length = 0;
    			    this->_letter = '\0';
			        return false;
			        }
			this->_length = bam_cigar_oplen(cigar[i]);
			this->_letter = bam_cigar_opchr(cigar[i]);
			return true;
			}

char CigarIterator::letter() {
    return _letter;
    }
int CigarIterator::length() {
    return _length;
    }
