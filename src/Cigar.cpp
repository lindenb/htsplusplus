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