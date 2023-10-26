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
