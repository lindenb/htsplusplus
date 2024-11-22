#include "GenomicSeq.hh"

GenomicSeq::GenomicSeq(Faidx* owner,const char* contig):CharSequence():owner(owner),interval(NULL) {
}

GenomicSeq::~GenomicSeq() {
}

CharSequence::size_t size() const {
    }

char at(CharSequence::size_t  i) const {
    if(interval==NULL || !interval->contains(interval->start()+i)) {
        
        }  
     return buffer[i - interval->start()];   
    }
