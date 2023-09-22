%{
/*
The MIT License (MIT)

Copyright (c) 2020 Pierre Lindenbaum

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

#include <cassert>
#include <cerrno>
#include <set>
#include <string>
#include "debug.hh"
#include "selgt.hh"
#include "selgt.lex.yy.hh"
#include "Regex.hh"

using namespace std;






void yyerror (SelectGTYYParam* ignore1,char const *s) {
   THROW_ERROR("[scanner]" << s << ".");
 }


%}



%parse-param {SelectGTYYParam* shuttle}

%define api.prefix {selgt}
%locations


%union {
    std::string* str;
    int sample_idx;
    std::set<int>* sample_indexes;
    std::set<int>* gt_types;
    CheckGt* checkGt;
    Node* node;
    FloatOrInt float_or_int;
	int d;
	float f;
	};

%token<d> LEX_GT_TYPE LEX_COLUMN_INDEX
%token<str> LEX_FILENAME
%token LEX_EQ
%token LEX_NE
%token LEX_GT LEX_GE LEX_LT LEX_LE
%token LEX_NO_CALL
%token LEX_AND
%token LEX_OR
%token LEX_NOT
%token LEX_CARET
%token LEX_STAR
%token<d> LEX_INT
%token<f> LEX_FLOAT
%token<str> LEX_STRING LEX_REGEX

%type<sample_idx> sample_index
%type<sample_indexes>  sample_items sample_set
%type<gt_types>  gt_type_set
%type<checkGt> sample_check0 sample_check
%type<node> boolean_expr and_expr or_expr
%type<d> opt_caret gt_type eqorne cmpop 
%type<float_or_int> samplecount

%%

input: or_expr {
	shuttle->returned_node =  $1;
	};


or_expr: and_expr {$$=$1;} | or_expr  LEX_OR and_expr {
	assert($1!=NULL);
	assert($3!=NULL);
	$$ = new Node();
	$$->type = Node::OR;
	$$->left = $1;
	$$->right = $3;
	};


and_expr: boolean_expr {$$=$1;} | and_expr  LEX_AND  boolean_expr {
	assert($1!=NULL);
	assert($3!=NULL);
	$$ = new Node();
	$$->type = Node::AND;
	$$->left = $1;
	$$->right = $3;
	};


boolean_expr: sample_check {
	$$ = new Node();
	$$->type = Node::COMPARE;
	$$->check = $1;
	} | '(' or_expr ')' {
	$$ = $2;
	}  | LEX_NOT or_expr {
	$$ = new Node();
	$$->type = Node::NOT;
	$$->left = $2;
	};
;

sample_check: sample_check0 {
	$$ = $1;
	$$->cmp_operator = LEX_EQ;
	$$->expect_n_samples = $$->samples->size();
	} | sample_check0 cmpop samplecount {
	std::set<int>::size_type expect_n_samples;
	$$ = $1;
	$1->cmp_operator = $2;
	if( $3.type == TYPE_IS_INT ) {
		expect_n_samples = $3.data.d;
		}
	else
		{
		expect_n_samples = (int)($3.data.f * $1->samples->size() );
		}
	if(expect_n_samples<0) {
		THROW_ERROR("Sample count cannot be lower than 0 (" << expect_n_samples << ")");
		}
	if(expect_n_samples> $1->samples->size()) {
		THROW_ERROR("Sample count ("<< expect_n_samples<<")cannot be greater than the number of samples in the group ("<< $1->samples->size() <<") " );
		}
	$$->expect_n_samples = expect_n_samples;
	};

sample_check0 : sample_set eqorne gt_type_set {
	$$ = new CheckGt;
	$$->negate = ($2==LEX_EQ?0:1);
	$$->samples = $1;
	$$->gtypes = $3;
	};

samplecount: LEX_INT {
	$$.type=TYPE_IS_INT;
	$$.data.d = $1;
	}|
	LEX_FLOAT {
	if($1<0.0 || $1>1.0) THROW_ERROR("Bad fraction of samples value 0<=" << $1 <<"<=1.0.");
	$$.type=TYPE_IS_FLOAT;
	$$.data.f = $1;
	}
	;

cmpop: eqorne {$$=$1;} | 
	LEX_LT {$$=LEX_LT;}|
	LEX_GT {$$=LEX_GT;}|
	LEX_GE {$$=LEX_GE;}|
	LEX_LE {$$=LEX_LE;};


eqorne:LEX_EQ {$$=LEX_EQ;} | LEX_NE {$$=LEX_NE;};



gt_type_set:gt_type {
    $$ = new set<int>;
   	$$->insert($1);
	}| gt_type_set '|' gt_type {
	$$->insert($3);
	};

gt_type: LEX_GT_TYPE {$$=$1;};

sample_set: sample_index {
	$$ = new set<int>;
	$$->insert($1);
	} | '[' opt_caret sample_items ']' {
	if($2==0)
		{
		$$ = $3;
		}
	else
		{
		int i;
		$$ = new std::set<int>;
		
		for(i=0;i< (int)bcf_hdr_nsamples(shuttle->vcf_header);i++) {
			if($3->find(i)!=$3->end()) continue;
			$$->insert(i);
			}
		delete $3;
		}
	} | opt_caret LEX_FILENAME {
	int negate=$1;
	char line[1024];
	FILE* f=fopen($2->c_str(),"r");
	if(f==NULL) {
		THROW_ERROR("Cannot open \""<< $2->c_str() << "\" (" << strerror(errno) << ")");
		}
	$$ =new set<int>;
	while(fgets(line,1024,f)!=NULL) {
		size_t l = strlen(line);
		if(l==0) continue;
		if(line[l-1]=='\n') line[l-1]=0;
		int idx = bcf_hdr_id2int(shuttle->vcf_header,BCF_DT_SAMPLE,line);
		if ( idx < 0 ) {
			WARNING("file \"" << $2 << "\" : undefined  sample \"" << line << "\" in vcf. SKIPPING.\n");
			continue;
			}
		$$->insert(idx);
		}
	fclose(f);
	if(negate==1) {
		int i;
		set<int>* array = new set<int>;
		for(i=0;i< (int)bcf_hdr_nsamples(shuttle->vcf_header);i++) {
			if($$->find(i)!=$$->end()) continue;
			array->insert(i);
			}
		delete $$;
		$$=array;
		}

	if($$->empty()) {
		THROW_ERROR("file \"" << line << "\" : no valid  sample found in vcf (negate flag=" << negate << ").");
		}
	free($2);
	} | opt_caret LEX_REGEX {
	
	int negate=$1;
	int i;
	std::unique_ptr<Regex> regex = Regex::compile($2->c_str(),REG_EXTENDED|REG_NOSUB);
	
	std::set<int>* array = new std::set<int>;
	for(i=0;i< (int)bcf_hdr_nsamples(shuttle->vcf_header);i++) {
		bool found =  regex->matches(shuttle->vcf_header->samples[i]);
		if(negate) found=!found;
		if(found) array->insert(i);
		}
	
	if(array->empty()) {
		THROW_ERROR("regex \"" << $2 << "\" : no valid  sample found in vcf  (negate flag=" << negate << ").");
		}
	delete $2;
	$$=array;
	} | LEX_STAR {
	int i;
	$$ = new set<int>;
	for(i=0;i< (int)bcf_hdr_nsamples(shuttle->vcf_header);i++) {
		$$->insert(i); 
		}
	};

sample_items: sample_index {
	$$ = new set<int>;
	$$->insert($1);
	} | sample_items  sample_index {
	$$->insert($2);
	};



sample_index : LEX_STRING {
	assert(shuttle->vcf_header!=NULL);
	assert($1!=NULL);

	int idx = bcf_hdr_id2int(shuttle->vcf_header,BCF_DT_SAMPLE,$1->c_str());
	if ( idx < 0 ) {
		THROW_ERROR("undefined  sample \"" << *($1) << "\" in vcf.");
		}
	delete $1;
	$$ = idx;
	} | LEX_COLUMN_INDEX {
	if($1 < 1 || $1>(int)bcf_hdr_nsamples(shuttle->vcf_header)) {
		THROW_ERROR("Bad sample index 1<=$" << ($1) << "<=" << bcf_hdr_nsamples(shuttle->vcf_header));
		}
	$$= ($1 -1 );
	} ;

opt_caret: {$$ = 0;} |  LEX_CARET { $$ = 1;};
%%


