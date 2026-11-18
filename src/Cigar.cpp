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

#include <cassert>
#include "Cigar.hh"
#include "debug.hh"

using namespace htspp;
using namespace std;



Cigar::Cigar() {
}


Cigar::Cigar(bam1_t* b) {
          assert(b!=NULL);
          CigarIterator iter(b);
          elements.reserve(b->core.n_cigar);
          while(iter.next()) {
                  elements.push_back(new CigarElement(iter.letter(), iter.length()));
                  }
          assert(elements.size()==b->core.n_cigar);
    }


                Cigar::~Cigar() {
                        for(unsigned int i=0;i< elements.size();i++) {
                                delete elements[i];
                                }
                        }
                bool Cigar::empty() const {
                        return elements.empty();
                        }
                unsigned int Cigar::size() const {
                        return elements.size();
                        }
                const CigarElement* Cigar::at(unsigned int idx) const {
                        return elements.at(idx);
                        }
                std::ostream& Cigar::print(std::ostream& out) const {
                        for(unsigned int i=0; i< size();i++) {
                                at(i)->print(out);
                                }
                        return out;
                        }


 std::shared_ptr<Cigar> Cigar::of(const char* s) {
  char* p=(char*)s;
  char* p2;
   assert(s!=0UL);
  errno=0;
   Cigar* cigar=new Cigar();
  while(*p!=0) {
    int len = strtol(p,&p2,10);
    assert(errno==0);
    char c = *p2;
    cigar->elements.push_back(new CigarElement(c, len));
    p=p2+1;
    }
  std::shared_ptr<Cigar> r(cigar);
  return r;
 }
 
 
 hts_pos_t Cigar::reference_length() const {
        int length = 0;
        for(auto i=elements.begin(); i!= elements.end();++i) {
            switch ((*i)->letter()) {
                case 'M':
                case 'D':
                case 'N':
                case '=':
                case 'X':
                    length += (*i)->length();
                    break;
                default: break;
            }
        }
        return length;
    }


 
 hts_pos_t Cigar::read_length() const {
        int length = 0;
        for(auto i=elements.begin(); i!= elements.end();++i) {
            if((*i)->op()->consumesReadBases()) {
            	length += (*i)->length();
            }
        }
        return length;
    }

 std::string Cigar::to_string() {
 		 std::ostringstream os;
 		 print(os);
 		 return os.str();
 }
 
bool Cigar::is_left_clipped() const {
	return !empty() && elements.at(0)->op()->is_clip();
  }


bool Cigar::is_right_clipped() const {
   return !empty() && elements.at(elements.size()-1)->op()->is_clip();
    }
 
 bool Cigar::is_clipped() const {
return is_left_clipped() || is_right_clipped();
 }
