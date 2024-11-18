#include <stdexcept>
#include <vector>
#include "GetOpt.hh"
#include "SelfTest.hh"
#include "Cigar.hh"
#include "Faidx.hh"
#include "HtsFile.hh"
#include "DiscreteMedian.hh"
#include "SamRecord.hh"
#include "SamFileReaderFactory.hh"
#undef ASSERT_NOT_NULL
using namespace std;
using namespace htspp;


#define ASSERT(a) try { tester->check(__FILE__,__LINE__,(a));} catch(...) {return;}
#define ASSERT_TRUE(a) ASSERT((a))
#define ASSERT_FALSE(a) ASSERT_TRUE((!(a)))
#define ASSERT_EQUALS(a,b) ASSERT_TRUE((a)==(b))
#define ASSERT_NOT_EQUALS(a,b) ASSERT_FALSE((a)==(b))
#define ASSERT_NULL(a) ASSERT_EQUALS((a),NULL)
#define ASSERT_NOT_NULL(a) ASSERT_NOT_EQUALS((a),NULL)
#define ASSERT_THROW(a) try { a  ; ASSERT(false)} catch(...) {ASSERT_TRUE(true)}

#define BEGIN_TEST try {
#define END_TEST } catch(std::exception& err) {cerr << err.what() << endl;} catch(...) {cerr << "BOUM catch(...) " << endl;}

static void testGetOpt1(SelfTest* tester) {
	BEGIN_TEST
	GetOpt obj;
	bool arg=false;
	const char* argv[]={"exe","-x"};
	obj.boolOpt('x',&arg,"desc");
	int v = obj.parse(2,(char**)argv);
	ASSERT_EQUALS(v,0);
	ASSERT_EQUALS(arg,true);
	ASSERT_TRUE(obj.files.empty());
	END_TEST
	}

static void testCigarOp(SelfTest* tester,const CigarElement* ce,hts_pos_t len,char c, const CigarOperator* op) {
	BEGIN_TEST
	ASSERT_NOT_NULL(ce);
	ASSERT_NOT_NULL(op);
	ASSERT_EQUALS(ce->letter(),c);
	ASSERT_EQUALS(ce->length(),len);
	ASSERT_EQUALS(ce->op(),op);
	END_TEST
	}

static void testCigar1(SelfTest* tester) {
	BEGIN_TEST
	const char* s="1S10M1I2D1M2H";
	static std::unique_ptr<Cigar> c= Cigar::of(s);
	ASSERT_NOT_NULL(c.get());
	ASSERT_EQUALS(c->size(),6);
	ASSERT_FALSE(c->empty());
	ASSERT_TRUE(c->is_left_clipped());
	ASSERT_TRUE(c->is_right_clipped());
	ASSERT_TRUE(c->is_clipped());
	ASSERT_EQUALS(c->reference_length(),13);
	ASSERT_EQUALS(c->read_length(),13);
	ASSERT_TRUE(c->to_string().compare(s)==0);
	testCigarOp(tester,c->at(0),1,'S',CigarOperator::S);
	testCigarOp(tester,c->at(1),10,'M',CigarOperator::M);
	testCigarOp(tester,c->at(2),1,'I',CigarOperator::I);
	testCigarOp(tester,c->at(3),2,'D',CigarOperator::D);
	testCigarOp(tester,c->at(4),1,'M',CigarOperator::M);
	testCigarOp(tester,c->at(5),2,'H',CigarOperator::H);
	END_TEST
	}

static void testCigarOperator1(SelfTest* tester) {
BEGIN_TEST
const CigarOperator* op = CigarOperator::of('M');
ASSERT_TRUE(op->consumesReadBases());
ASSERT_TRUE(op->consumesReferenceBases());
ASSERT_FALSE(op->is_clip());
ASSERT_TRUE(op->is_align());

op = CigarOperator::of('X');
ASSERT_TRUE(op->consumesReadBases());
ASSERT_TRUE(op->consumesReferenceBases());
ASSERT_FALSE(op->is_clip());
ASSERT_TRUE(op->is_align());

op = CigarOperator::of('=');
ASSERT_TRUE(op->consumesReadBases());
ASSERT_TRUE(op->consumesReferenceBases());
ASSERT_FALSE(op->is_clip());
ASSERT_TRUE(op->is_align());


op = CigarOperator::of('I');
ASSERT_TRUE(op->consumesReadBases());
ASSERT_FALSE(op->consumesReferenceBases());
ASSERT_FALSE(op->is_clip());
ASSERT_FALSE(op->is_align());

op = CigarOperator::of('D');
ASSERT_FALSE(op->consumesReadBases());
ASSERT_TRUE(op->consumesReferenceBases());
ASSERT_FALSE(op->is_clip());
ASSERT_FALSE(op->is_align());


op = CigarOperator::of('N');
ASSERT_FALSE(op->consumesReadBases());
ASSERT_TRUE(op->consumesReferenceBases());
ASSERT_FALSE(op->is_clip());
ASSERT_FALSE(op->is_align());


op = CigarOperator::of('P');
ASSERT_FALSE(op->consumesReadBases());
ASSERT_FALSE(op->consumesReferenceBases());
ASSERT_FALSE(op->is_clip());
ASSERT_FALSE(op->is_align());

op = CigarOperator::of('S');
ASSERT_TRUE(op->consumesReadBases());
ASSERT_FALSE(op->consumesReferenceBases());
ASSERT_TRUE(op->is_clip());
ASSERT_FALSE(op->is_align());

op = CigarOperator::of('H');
ASSERT_FALSE(op->consumesReadBases());
ASSERT_FALSE(op->consumesReferenceBases());
ASSERT_TRUE(op->is_clip());
ASSERT_FALSE(op->is_align());
END_TEST
}


SelfTest::SelfTest():n_passing(0L),n_fail(0L) {
}
static void testFaidx1(SelfTest* tester) {
	BEGIN_TEST
	std::unique_ptr<Faidx> obj = Faidx::load("/home/lindenb/src/htsplusplus/tests/toy.fa");
	ASSERT_NOT_NULL(obj.get());
	ASSERT_EQUALS(obj->nseq(),2);
	ASSERT_EQUALS(strcmp(obj->name(0),"ref"),0);
	ASSERT_EQUALS(strcmp(obj->name(1),"ref2"),0);
	std::shared_ptr<SamSequenceDictionary> dict = obj->dictionary();
	ASSERT_NOT_NULL(dict.get());
  ASSERT_EQUALS(dict->nseq(),2);
  const SamSequenceRecord* ssr=dict->getSequence("ref");
  ASSERT_NOT_NULL(ssr);
  ASSERT_EQUALS(ssr->tid(),0);
  ASSERT_EQUALS(ssr->length(),45);
  
  ssr=dict->getSequence("ref2");
  ASSERT_NOT_NULL(ssr);
  ASSERT_EQUALS(ssr->tid(),1);
  ASSERT_EQUALS(ssr->length(),40);
  
  ASSERT_EQUALS(dict->genome_length(),85);
  
  
  std::unique_ptr<Locatable> r= dict->parseInterval("chrref:4-100");
  ASSERT_NOT_NULL(r.get());
  ASSERT_EQUALS(strcmp(r->contig(),"ref"),0);
  ASSERT_EQUALS(r->start(),4);
  ASSERT_EQUALS(r->end(),45);
  
  ASSERT_THROW(dict->parseInterval("ref:10000-1"));
  ASSERT_THROW(dict->parseInterval("chr2:1-2"));
	END_TEST
}

static void testHtsFile1(SelfTest* tester) {
	BEGIN_TEST
	std::unique_ptr<HtsFile> obj = HtsFile::open("/home/lindenb/src/htsplusplus/tests/toy.bam");
	ASSERT_NOT_NULL(obj.get());
	ASSERT_TRUE(obj->is_open());
	
	obj->close();
	ASSERT_FALSE(obj->is_open());
	END_TEST
}


static void testOptional1(SelfTest* tester) {
	BEGIN_TEST
	Optional<int> o1 = make_optional(13);
	Optional<int> e1 = make_empty_optional<int>();
	ASSERT_TRUE(o1.isPresent());
	ASSERT_FALSE(e1.isPresent());
	ASSERT_EQUALS(o1.get(),13);
	ASSERT_EQUALS(o1.to_string(),"13");
	ASSERT_EQUALS(o1.orElse(28),13);
	ASSERT_EQUALS(e1.orElse(28),28);
	ASSERT_TRUE(o1==o1);
	ASSERT_TRUE(e1==e1);
	ASSERT_FALSE(o1==e1);
	ASSERT_TRUE(o1!=e1);
	
	e1=o1;
	ASSERT_TRUE(o1==e1);
	
	END_TEST
	}

static void testDiscreteMedian1(SelfTest* tester) {
	BEGIN_TEST
	DiscreteMedian<int> dm;
	ASSERT_TRUE(dm.empty());
	for(int i=0;i< 10;i++) dm.accept(27);
	dm.accept(1);
	dm.accept(10000000);
	ASSERT_FALSE(dm.empty());
	Optional<double> m = dm.median();
	ASSERT_TRUE(m.isPresent());
	ASSERT_EQUALS((int)m.get(),27);
	END_TEST
	}
	
static void testDiscreteMedian2(SelfTest* tester) {
	BEGIN_TEST
	DiscreteMedian<int> dm;
	ASSERT_TRUE(dm.empty());
	for(int i=0;i< 10000;i++) {
		dm.accept(100);
		dm.accept(102);
		}
	ASSERT_EQUALS((int)dm.median().get(),101);
	END_TEST
	}

static void testDiscreteMedian3(SelfTest* tester) {
	BEGIN_TEST
	DiscreteMedian<int> dm;
	dm.accept(1);
	dm.accept(2);
	ASSERT_EQUALS(dm.median().get(),1.5);
	END_TEST
	}
	
static void testSamReader1(SelfTest* tester) {
	BEGIN_TEST
	std::vector<struct AlignmentBlock> blocks;
	std::string fasta("/home/lindenb/src/htsplusplus/tests/toy.fa");
	std::string cram("/home/lindenb/src/htsplusplus/tests/toy.cram");
	for(int with_index=0;with_index < 2;++with_index) {
		SamFileReaderFactory sfr;
		sfr.reference(fasta.c_str());
		auto sr = sfr.open(cram.c_str(),with_index==1);
		ASSERT_NOT_NULL(sr.get());
		auto iter= sr->iterator();
		ASSERT_NOT_NULL(iter.get());
		SamRecord rec;
		
		int n=0;
		while(iter->read2(&rec)) {
			switch(n) {
					case 0:
						ASSERT_EQUALS(strcmp(rec.read_name(),"r001"),0);
						ASSERT_EQUALS(strcmp(rec.contig(),"ref"),0);
						ASSERT_EQUALS(rec.flags(),163);
						ASSERT_EQUALS(rec.start(),7);
						ASSERT_EQUALS(rec.mapq(),30);
						ASSERT_TRUE(rec.has_cigar());
						ASSERT_EQUALS(rec.cigar_string().compare("8M4I4M1D3M"),0);
						

						rec.alignment_blocks(blocks);
						ASSERT_EQUALS((int)blocks.size(),3);
						ASSERT_EQUALS((int)blocks[0].length,8);
						ASSERT_EQUALS((int)blocks[0].referenceStart,7);
						
						ASSERT_EQUALS((int)blocks[1].length,4);
						ASSERT_EQUALS((int)blocks[1].referenceStart,15);

						ASSERT_EQUALS((int)blocks[2].length,3);
						ASSERT_EQUALS((int)blocks[2].referenceStart,20);
					
						break;
					default:
						break;
					}
			n++;
			}
		ASSERT_EQUALS(n,12);
		}
	END_TEST
	}


static void testSamReader2(SelfTest* tester) {
	BEGIN_TEST
	for(int with_index=0;with_index < 2;++with_index) {
		std::string cram("/home/lindenb/src/htsplusplus/tests/toy.bam");
		SamFileReaderFactory sfr;
		auto sr = sfr.open(cram.c_str(),with_index==1);
		ASSERT_NOT_NULL(sr.get());
		auto iter= sr->iterator();
		ASSERT_NOT_NULL(iter.get());
		SamRecord rec;
		int n=0;
		while(iter->read2(&rec)) {
			n++;
			}
		ASSERT_EQUALS(n,12);
		}
	END_TEST
	}
	


void SelfTest::check(const char* fname,int line,bool b) {
	if(b) {
	 	n_passing++;
		}
	else
		{
		n_fail++;
		cerr << "FAIL at " << fname << ":" << line << endl;
		throw std::runtime_error("failing");
		}
	}

int SelfTest::run(const char* base) {
	testGetOpt1(this);
	testCigar1(this);
	testCigarOperator1(this);
	testFaidx1(this);
	testHtsFile1(this);
	testOptional1(this);
	testDiscreteMedian1(this);
	testDiscreteMedian2(this);
	testDiscreteMedian3(this);
	testSamReader1(this);
	testSamReader2(this);
	cerr << "Tests done. PASS " << n_passing << " FAIL: " << n_fail << endl;
	return (int)n_fail;
	}



