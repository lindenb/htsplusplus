#ifndef GENOMIC_LOC_PARSER_HH
#define GENOMIC_LOC_PARSER_HH

#include "Locatable.hh"
#include "SamSequenceDictionary.hh"

namespace htspp {

class GenomicLocParser {
private:
	const SamSequenceDictionary* dict;
	bool _trim_pos;
	bool _resolve_contig;
public:
	GenomicLocParser();
	GenomicLocParser(const SamSequenceDictionary* dict);
	~GenomicLocParser();
	GenomicLocParser& resolve_contigs(bool b);
	GenomicLocParser& trim_positions(bool b);
	std::unique_ptr<Locatable> parse(const char* s) const;

};

}



#endif
