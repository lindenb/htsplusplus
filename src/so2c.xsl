<?xml version='1.0'  encoding="UTF-8" ?>
<xsl:stylesheet
	xmlns:xsl='http://www.w3.org/1999/XSL/Transform'
	xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
	xmlns:owl="http://www.w3.org/2002/07/owl#"
	xmlns:oboInOwl="http://www.geneontology.org/formats/oboInOwl#"
	xmlns:xsd="http://www.w3.org/2001/XMLSchema#"
	xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#"
	xmlns:obo="http://purl.obolibrary.org/obo/"
        version='1.0'
        >
<!--
Author:
        Pierre Lindenbaum

-->

<xsl:output method="text" />

<xsl:template match="/">

#include &lt;set&gt;
#include &lt;map&gt;
#include &lt;vector&gt;
#include &lt;string&gt;
#include &lt;algorithm&gt;
#include "debug.hh"
#include "SequenceOntology.hh"

#define PURL "http://purl.obolibrary.org/obo/"

class TermImpl :public SequenceOntology::Term {
	public:
		std::string _acn;
		std::string _label;
		std::set&lt;TermImpl const*&gt; _children;
		TermImpl() {}
		virtual ~TermImpl() {}
		virtual const char* acn() const { return _acn.c_str();}
		virtual const char* label() const { return _label.c_str();}
		virtual bool has_descendant(const Term* child_node) const {
				if(child_node==this) return true;
				for(auto r=_children.begin(); r!=_children.end();++r) {
					if((*r)->has_descendant(child_node)) return true;
					}
				return false;
				}
		virtual void collect(std::set&lt;SequenceOntology::Term const*&gt;&amp; terms) const {
				terms.insert(this);
				for(auto r=_children.begin(); r!=_children.end();++r) {
					(*r)->collect(terms);
					}
				}
	};

class SequenceOntologyImpl: public SequenceOntology {
	public:
		
		std::map&lt;std::string,TermImpl*&gt; label2term;
		std::map&lt;std::string,TermImpl*&gt; acn2term;
		
		SequenceOntologyImpl() {
			}
		virtual ~SequenceOntologyImpl() {
			for(auto r=acn2term.begin();r!=acn2term.end();++r) delete r->second;
			}
		virtual	const Term* find_by_acn(const char* s) const {
			auto r = acn2term.find(s);
			return r==acn2term.end()?NULL:r->second;
			}
		virtual const Term* find_by_label(const char* s) const {
			std::string q(s);
			auto r = label2term.find(q);
			if(r!=label2term.end()) return r->second;
			std::replace(q.begin(), q.end(), '_',' ');
			r = label2term.find(q);
			if(r!=label2term.end()) return r->second;
			return NULL;
			}
};

class Builder {

std::map&lt;std::string,TermImpl*> uri2term;
std::vector&lt;TermImpl *&gt; father2child;

TermImpl* insert(SequenceOntologyImpl* ontology,const char* uri, const char* acn, const char* label) {
	auto r = uri2term.find(uri);
	TermImpl* t;
	if(r==uri2term.end()) {
		t = new TermImpl();
		uri2term.insert(std::make_pair(uri,t));
		} else {
		t = r->second;
		}
	if(acn!=NULL &amp;&amp; t->_acn.empty()) t->_acn.assign(acn);
	if(label!=NULL &amp;&amp; t->_label.empty()) t->_label.assign(label);
	if(!t->_label.empty()) {
		ontology->label2term.insert(std::make_pair(t->_label,t));
		}
	if(!t->_acn.empty()) {
		ontology->acn2term.insert(std::make_pair(t->_acn,t));
		}
	return t;
	}
void children(SequenceOntologyImpl* ontology,TermImpl* t, const char* curi) {
	TermImpl* t2 = insert(ontology,curi,NULL,NULL);
	if(t==t2) THROW_ERROR("illegal state");
	father2child.push_back(t);
	father2child.push_back(t2);
	}

public:

SequenceOntology* make() {
	SequenceOntologyImpl* ontology = new SequenceOntologyImpl;
	TermImpl* t;
<xsl:apply-templates select="/rdf:RDF/owl:Class[@rdf:about='http://purl.obolibrary.org/obo/SO_0001060']"/>
	for(unsigned int i=0;i&lt; father2child.size();i+=2) {
		TermImpl* t1 = father2child[i+0];
		TermImpl* t2 = father2child[i+1];
		if(t2->_acn.empty())  THROW_ERROR("illegal state");
		if(t2->_label.empty())  THROW_ERROR("illegal state");
		t1->_children.insert(t2);
		}
	
	return ontology;
	};
};


std::unique_ptr&lt;SequenceOntology&gt; SequenceOntology::make() {
	Builder b;
	return std::unique_ptr&lt;SequenceOntology&gt;(b.make());
	}

</xsl:template>



<xsl:template match="owl:Class">
<xsl:variable name="uri" select="@rdf:about"/>
t = insert(ontology,"<xsl:value-of select="$uri"/>","<xsl:value-of select="oboInOwl:id/text()"/>","<xsl:value-of select="rdfs:label/text()"/>");
<xsl:for-each select="/rdf:RDF/owl:Class[rdfs:subClassOf/@rdf:resource = $uri and not(owl:deprecated='true')]">

children(ontology,t,"<xsl:value-of select="@rdf:about"/>");
</xsl:for-each>
<xsl:apply-templates select="/rdf:RDF/owl:Class[rdfs:subClassOf/@rdf:resource = $uri and not(owl:deprecated='true')]"/>
</xsl:template>




</xsl:stylesheet>

