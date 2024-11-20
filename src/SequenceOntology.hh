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

#ifndef SEQUENCE_ONTOLOGY_HH
#define SEQUENCE_ONTOLOGY_HH
#include <memory>
#include <set>
class SequenceOntology {
	protected:
		SequenceOntology() {}
	public:
		class Term {
			public:
				Term() {}
				virtual ~Term() {}
				virtual const char* acn() const = 0;
				virtual const char* label() const = 0;
				virtual bool has_descendant(const Term* child_node) const=0;
				virtual void collect(std::set<const Term*>& terms) const =0;
			};
		virtual ~SequenceOntology() {}
		virtual const SequenceOntology::Term* find_by_acn(const char* s)  const = 0 ;
		virtual const SequenceOntology::Term* find_by_label(const char* s) const = 0 ;
		static std::unique_ptr<SequenceOntology> make();
	};


#endif