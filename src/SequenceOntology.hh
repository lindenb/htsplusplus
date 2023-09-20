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