#ifndef GENOMIC_SEQ_HH
#define GENOMIC_SEQ_HH
#include "Locatable.hh"
#include "CharSequence.hh"
namespace htspp {

class GenomicSeq : public CharSequence {
private:
    Faidx* owner;
    mutable Locatable* interval;
public:
    GenomicSeq(Faidx* owner,const char* contig);
    virtual ~GenomicSeq();
    virtual CharSequence::size_t size() const;
    virtual char at(CharSequence::size_t  i) const;
};

}

#endif

