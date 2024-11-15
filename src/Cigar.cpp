#include <cassert>
#include "Cigar.hh"
#include "debug.hh"

using namespace htspp;

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

 std::unique_ptr<Cigar> Cigar::of(const char* s) {
  std::unique_ptr<Cigar> cig(new Cigar());
  char* p=(char*)s;
  char* p2;
  errno=0;
  while(*p!=0) {
    int len = strtol(p,&p2,10);
    assert(errno==0);
    char c = *p2;
    cig->elements.push_back(new CigarElement(c, len));
    p=p2+1;
    }
  return cig;
 }
