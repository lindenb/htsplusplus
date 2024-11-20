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

#ifndef CONTIG_POS_HH
#define CONTIG_POS_HH
#include <string>
#include <iostream>
#include "Comparable.hh"
#include "Locatable.hh"

class ContigPos : public Locatable,Comparable<ContigPos> {
	private:
		std::string ctg;
		hts_pos_t _pos;
	public:
		ContigPos(const char* c,hts_pos_t p):ctg(c),_pos(p) {
			}
		ContigPos(const ContigPos& cp):ctg(cp.ctg),_pos(cp._pos) {
			}
		virtual ~ContigPos() {
			}
		virtual const char* contig() const { return ctg.c_str();}
		virtual hts_pos_t position() const { return _pos;}
		virtual hts_pos_t start() const { return position();}
		virtual hts_pos_t end() const { return position();}
		virtual int compareTo(const ContigPos& cp) const {
			int i = std::strcmp(contig(),cp.contig());
			if(i!=0) return i;
			return position() - cp.position();
			}
			
		MAKE_DEFAULT_COMPARABLE_METHODS(ContigPos)
		
		static ContigPos of(std::string s);
	};

std::ostream& operator << (std::ostream& out,const ContigPos& o);

#endif
