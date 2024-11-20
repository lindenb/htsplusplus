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
