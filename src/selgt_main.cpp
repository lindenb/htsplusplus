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
#include <set>
#include <getopt.h>
#include <unistd.h>
#include <cctype>
#include "programs.hh"
#include "debug.hh"
#define YY_DECL int yylex(YYSTYPE * yylval_param, yyscan_t yyscanner, int *pint)
#include "selgt.hh"
#include "selgt.tab.hpp"
#include "selgt.lex.yy.hh"
#include "BcfReader.hh"
#include "BcfWriter.hh"

using namespace std;

extern int selgtparse(bcf_hdr_t* yyparse,Node** returned_node);

static int int_compare(int a, int op,int b) {
	switch(op) {
		case LEX_EQ : return a==b;
		case LEX_NE : return a!=b;
		case LEX_LT : return a < b;
		case LEX_LE : return a <= b;
		case LEX_GE : return a>=b;
		case LEX_GT : return a>b;
		default: THROW_ERROR("Illegal state " << op);
		}
	}

static bool eval_variant(vector<int>* gts,Node* node) {
	int matching=0;
	assert(gts!=NULL);
	assert(node!=NULL);
	
	switch(node->type) {
		case Node::COMPARE:
				{
				for(set<int>::iterator i=node->check->samples->begin();
					i !=  node->check->samples->end() ;
					++i) {
					int sample_index= *i;
					int sample_gt=  gts->at(sample_index);
					int found = node->check->gtypes->find(sample_gt)!= node->check->gtypes->end();
					
					if(node->check->negate && !found) {
						matching++;	
						}
					else if(!node->check->negate && found) {
						matching++;	
						}
					}
				return int_compare(matching, node->check->cmp_operator,node->check->expect_n_samples);
				}
		case Node::AND:
			return eval_variant(gts,node->left) && eval_variant(gts,node->right);
		case Node::OR:
			return eval_variant(gts,node->left) || eval_variant(gts,node->right);
		case Node::NOT:
			return !eval_variant(gts,node->left);
		default: THROW_ERROR("bad state"); abort(); break;
		}
	return false;
	}

    
int main_bcffiltergt(int argc,char** argv) {
BcffiltergtArgs args;
if(!args.parse(argc,argv)) {
	return EXIT_FAILURE;
	}
PROGRAM_COMMON(args);

std::string script;

if( args.user_expr_str !=NULL &&  args.script_file !=NULL) {
    THROW_ERROR("both option -e and -f were used");
    return EXIT_FAILURE;    
	}
else if( args.user_expr_str ==NULL &&  args.script_file ==NULL ) {
    THROW_ERROR("expression (or script file) is missing");
    return EXIT_FAILURE;    
	}
else if(args.script_file!=NULL)
	{
	script.assign(IoUtils::slurpFile(args.script_file));
	}
else
	{
	script.assign(args.user_expr_str);
	}
	
//cleanup script
for(unsigned int i=0;i< script.size();i++) {
	if(isspace(script[i])) script[i]=' ';
	i++;
	}

std::unique_ptr<BcfReader> in = BcfReader::open(optind==argc ?"-":argv[optind]);
BcfWriterFactory writer_factory;
std::unique_ptr<BcfWriter> out = writer_factory.format(args.bcf_output_format).open(args.output_filename);

BcfHeader::filter_type filter_id=-1;
std::unique_ptr<BcfHeader> out_header =  in->header->clone();
if(!StringUtils::isBlank(args.soft_filter_name)) {
	filter_id = out_header->add_filter(args.soft_filter_name);
	}

out_header->printf("##%s.command=%s (version:%s)",
	args.program_name(),
	script.c_str(),
	args.program_version()
	);

unsigned int nsmpl = in->header->nsamples();
if( nsmpl ==0UL ) {
	THROW_ERROR("No Genotypes in input.");
	return EXIT_FAILURE;
	}

out->write_header(in->header->get());




SelectGTYYParam myparam;
myparam.vcf_header = in->header->get();
myparam.returned_node = NULL; 
YY_BUFFER_STATE buffer_state = selgt_scan_string(args.user_expr_str);
selgtparse(&myparam);
selgt_delete_buffer(buffer_state);
if( myparam.returned_node == NULL) {
 	THROW_ERROR("Illegal state. Node is null");
    return EXIT_FAILURE;   
	}

BcfRecordHeader* bcf = new BcfRecordHeader;


vector<int>* genotype_types  = new vector<int>;
for(unsigned int i=0;i< nsmpl ;i++) {
	genotype_types->push_back(-1);
	}
assert(genotype_types->size() == nsmpl);
	    
while(in->read2(bcf)) {
	bcf_fmt_t *gt_fmt = NULL;

   	bcf->unpack_genotypes();
    if ( (gt_fmt=( bcf->get_fmt("GT"))) ==NULL ) continue;

    for(unsigned int i=0;i< nsmpl ;i++) {
		int type = bcf_gt_type(gt_fmt,i,NULL,NULL);
		genotype_types->at(i)=type;
		}
	bool keep =  eval_variant(genotype_types,myparam.returned_node);
	

	if(!keep) {
		if(filter_id<0) continue;
		bcf->add_filter(filter_id);
		}
	out->write2(bcf);
  	}

delete bcf;
delete (myparam.returned_node);

return EXIT_SUCCESS;
}
