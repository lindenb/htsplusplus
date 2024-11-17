#include <stdexcept>
#include "GetOpt.hh"
#include "SelfTest.hh"
#include "Cigar.hh"
using namespace std;
using namespace htspp;


#define ASSERT(a) try { tester->check(__FILE__,__LINE__,(a));} catch(...) {return;}
#define ASSERT_TRUE(a) ASSERT((a))
#define ASSERT_FALSE(a) ASSERT_TRUE((!(a)))
#define ASSERT_EQUALS(a,b) ASSERT_TRUE((a)==(b))
#define ASSERT_NOT_EQUALS(a,b) ASSERT_FALSE((a)==(b))
#define ASSERT_NULL(a) ASSERT_EQUALS((a),NULL)
#define ASSERT_NOT_NULL(a) ASSERT_NOT_EQUALS((a),NULL)

#define BEGIN_TEST try {
#define END_TEST } catch(...) {cerr << "BOUM" << endl;}

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

static void tesCigar1(SelfTest* tester) {
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

static void tesCigarOperator1(SelfTest* tester) {
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
	tesCigar1(this);
	tesCigarOperator1(this);
	cerr << "Tests done. PASS " << n_passing << " FAIL: " << n_fail << endl;
	return (int)n_fail;
	}



