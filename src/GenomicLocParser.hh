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
	bool _contig_only;
	bool _rm_commas;
	bool _with_suffix;
	hts_pos_t parseInt(const char* src,std::string s) const;
public:
	GenomicLocParser();
	GenomicLocParser(const SamSequenceDictionary* dict);
	~GenomicLocParser();
	GenomicLocParser& resolve_contigs(bool b);
	GenomicLocParser& enable_contig_only(bool b);
	GenomicLocParser& trim_positions(bool b);
	GenomicLocParser& remove_commas(bool b);
	GenomicLocParser& enable_suffix(bool b);
	std::unique_ptr<Locatable> parse(const char* s) const;

};

}



#endif
