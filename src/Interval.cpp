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

#include <cstring>
#include "Interval.hh"
#include "GenomicLocParser.hh"
#include "debug.hh"
using namespace std;
using namespace htspp;

Interval::Interval(const char* c,hts_pos_t start,hts_pos_t end):ctg(c),beg(start),stop(end) {
			}
Interval::Interval(const Interval& cp):ctg(cp.ctg),beg(cp.beg),stop(cp.stop) {
			}
Interval::Interval(const Locatable* o):ctg(o->contig()),beg(o->start()),stop(o->end()) {
			}
Interval::~Interval() {
			}
const char* Interval::contig() const { return ctg.c_str();}
hts_pos_t Interval::start() const { return beg;}
hts_pos_t Interval::end() const { return stop;}
int Interval::compareTo(const Interval& cp) const {
			int i = std::strcmp(contig(),cp.contig());
			if(i!=0) return i;
			i= start() - cp.start();
			if(i!=0) return i;
			return end() - cp.end();
			}
		


std::ostream& operator << (std::ostream& out,const Interval& o) {
    out << o.contig() << ":" << o.start() << "-" << o.end();
    return out;    
    }
