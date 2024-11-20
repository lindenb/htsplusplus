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

#include "CoordMath.hh"
#include "Locatable.hh"

using namespace htspp;


Locatable::Locatable() {}
Locatable::~Locatable() {}

 bool Locatable::has_contig(const char* ctg) const {
	return ctg!=NULL && std::strcmp(ctg,contig())==0;
	}
 bool Locatable::within_distance_of(Locatable* other, hts_pos_t distance) const {
	if(!has_contig(other->contig())) return false;
	return CoordMath::overlaps(start(), end(), other->start()-distance, other->end()+distance);
	}
 hts_pos_t Locatable::length_on_reference() const {
	return  CoordMath::getLength(start(),end()); 
	}
		
 std::ostream& Locatable::print(std::ostream& out) const {
	out << contig() << ":" << start() << "-" << end();
	return out;
	}
			
 std::string Locatable::to_string() const {
	std::ostringstream os;
	print(os);
	return os.str();
	}

bool Locatable::contains_position(hts_pos_t pos) const {
    return start() <= pos && pos <= end();
}
