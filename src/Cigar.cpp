#include "Cigar.hh"
#include "debug.hh"


using namespace htspp;

               	Cihar::Cigar(bam1_t* b) {
                        CigarIterator iter(b);
                        elements.reserve(b->core.n_cigar);
                        while(iter.next()) {
                                elements.push_back(new CigarElement(iter.letter, iter.length));
                                }
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
                CigarElement* Cigar::at(unsigned int idx) const {
                        return elements.at(idx);
                        }
                std::ostream& Cigar::print(std::ostream& out) const {
                        for(unsigned int i=0; i< size();i++) {
                                at(i)->print(out);
                                }
                        return out;
                        }

