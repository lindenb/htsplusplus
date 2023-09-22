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

#ifndef SEL_GT_H
#define SEL_GT_H
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <vector>
#include <htslib/hts.h>
#include <htslib/vcf.h>
#include <htslib/vcfutils.h>

using namespace std;



#define TYPE_IS_INT 0
#define TYPE_IS_FLOAT 1
typedef struct float_or_int_t {
	int type;
	union {
		int d;
		double f;
		} data;
	} FloatOrInt;


class CheckGt {
	public:
		bool negate;
		int cmp_operator;
		std::set<int>::size_type expect_n_samples;
		std::set<int>* samples;
		std::set<int>* gtypes;
		CheckGt():negate(false),cmp_operator(0),expect_n_samples(0UL),samples(NULL),gtypes(NULL){
			}
		~CheckGt() {
			if(samples!=NULL) delete (this->samples);
			if(gtypes!=NULL) delete (this->gtypes);
			}
	};



class Node {
	public:
		enum {COMPARE=0,AND=1,OR=2,NOT=3};
		int type;
		Node* left;
		Node* right;
		CheckGt* check;
		Node():type(0),left(NULL),right(NULL),check(NULL) {
			}
		~Node() {
			switch(this->type) {
				case Node::COMPARE:
					delete check;
					break;
				case Node::AND : case Node::OR:
					delete left;
					delete right;
					break;
				case Node::NOT:
					delete left;
					break;
				}
			}
	};

struct SelectGTYYParam {
	bcf_hdr_t* vcf_header;
	Node* returned_node;
	};


#endif
