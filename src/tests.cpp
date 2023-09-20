#include <string>
#include "programs.hh"
#include "utils.hh"
#include "debug.hh"
#include "RegIdx.hh"
#include "SequenceOntology.hh"
using namespace std;

#define ASSERT_EQUALS(a,b) if((a)!=(b)) {THROW_ERROR("assert " #a "==" #b " Failed. a=" << a << " b=" << b);} else {DEBUG("EQUALS "#a" ==" #b);}
#define ASSERT_TRUE(a) if(!(a))  {THROW_ERROR("assert true " #a " Failed");} else {DEBUG("ASSERT_TRUE "#a);}
#define ASSERT_FALSE(a) if((a))  {THROW_ERROR("assert false " #a " Failed");} else {DEBUG("ASSERT_FALSE "#a);}

struct Command{
	unsigned int n;
	char** array;
	char** cmd;
	};
	
static Command* create_array(vector<string>& v) {
Command* ptr = (Command*)malloc(sizeof(Command));
ASSERT_NOT_NULL(ptr);
ptr->n = v.size();
ptr->array=(char**)malloc(sizeof(char*)*(v.size()+1));
ASSERT_NOT_NULL(ptr->array);
ptr->cmd=(char**)malloc(sizeof(char*)*(v.size()+1));
ASSERT_NOT_NULL(ptr->cmd);
for(unsigned int i=0;i< v.size();i++) {
	ptr->array[i] = strdup(v[i].c_str());
	cerr << ptr->array[i]  << " ";
	ASSERT_NOT_NULL(ptr->array[i]);
	ptr->cmd[i] = ptr->array[i];
	}
ptr->array[v.size()]=0;
ptr->cmd[v.size()]=0;
cerr << endl;
return ptr;
}

static void dispose_array(Command* ptr) {
	ASSERT_NOT_NULL(ptr);
	for(unsigned int i=0;i< ptr->n;i++) {
		free(ptr->array[i]);
		}
	free(ptr->array);
	free(ptr->cmd);
	free(ptr);
	}

static int callback2(const char* chr, int beg, int end,void* usr) {
 	int* n=(int*)usr;
 	DEBUG(chr<< " " << beg << " " << end);
 	switch(*n) {
 		case 0:
	 		ASSERT_TRUE(strcmp(chr,"chr1")==0)
	 		ASSERT_EQUALS(beg,11)
	 		ASSERT_EQUALS(end,20)
	 		break;
	 	case 1:
	 		ASSERT_TRUE(strcmp(chr,"chr1")==0)
	 		ASSERT_EQUALS(beg,101)
	 		ASSERT_EQUALS(end,200)
	 		break;
	 	case 2:
	 		ASSERT_TRUE(strcmp(chr,"chr1")==0)
	 		ASSERT_EQUALS(beg,1001)
	 		ASSERT_EQUALS(end,2000)
	 		break;
	 	default: return -1; break;
 		}
 	++*n;
 	return 0;
 	}

static void test_bed_01(string datadir) {
	DEBUG(__FUNCTION__);
	string fname;
	fname.assign(datadir).append( IoUtils::separator() ).append("test.01.bed");
	IoUtils::assertFileExist(fname.c_str());
	std::unique_ptr<RegIdx> idx = RegIdx::load_file(fname.c_str());
	ASSERT_EQUALS(idx->count(),6L);
	int n=0;
	idx->for_each(callback2,(void*)&n);
	ASSERT_EQUALS(n,3);
	ASSERT_FALSE(idx->overlaps("chrX",10,10))
	ASSERT_TRUE(idx->overlaps("chr1",1,1000))
	ASSERT_FALSE(idx->overlaps("chr1",1,10))
	ASSERT_FALSE(idx->overlaps("chr1",10,10))
	ASSERT_TRUE(idx->overlaps("chr1",1,11))
	ASSERT_TRUE(idx->overlaps("chr1",11,11))
	ASSERT_TRUE(idx->overlaps("chr1",20,20))
	ASSERT_FALSE(idx->overlaps("chr1",21,21))
	ASSERT_EQUALS(idx->count("chrX",21,21),0)
	ASSERT_EQUALS(idx->count("chr1",1,100000),3)
	ASSERT_EQUALS(idx->count("chr1",1,10),0)
	ASSERT_EQUALS(idx->count("chr1",11,11),1)
	ASSERT_EQUALS(idx->count("chr1",20,20),1)
	ASSERT_EQUALS(idx->count("chr1",21,21),0)
	ASSERT_EQUALS(idx->count("chr1",101,101),1)
	}

static void test_samviewwithmate_01(string datadir) {
	DEBUG(__FUNCTION__);
	string bedname;
	string samname;
	bedname.assign(datadir).append( IoUtils::separator() ).append("viewwithmate.01.bed");
	samname.assign(datadir).append( IoUtils::separator() ).append("viewwithmate.01.sam");
	vector<string> cmd;
	cmd.push_back("samviewwithmate");
	cmd.push_back("-O");
	cmd.push_back("SAM");
	cmd.push_back("-B"); cmd.push_back(bedname);
	cmd.push_back(samname);
	Command* args = create_array(cmd);
	ASSERT_EQUALS(main_samviewwithmate(args->n,args->cmd),0);
	dispose_array(args);
	}

static void test_sequence_ontology() {
	DEBUG("build SO");
	 std::unique_ptr<SequenceOntology> so = SequenceOntology::make();
	 DEBUG("end SO");
	 const SequenceOntology::Term* t1 = so->find_by_acn("SO:0001818");
	 ASSERT_NOT_NULL(t1);
	 ASSERT_TRUE(strcmp(t1->label(),"protein_altering_variant")==0);
	 
	 const SequenceOntology::Term* t2 = so->find_by_label("protein_altering_variant");
	 ASSERT_NOT_NULL(t2);
	 ASSERT_TRUE(t1==t2);
	 
	 const SequenceOntology::Term* t3 = so->find_by_acn("SO:0001591");
	 ASSERT_NOT_NULL(t3);
	 ASSERT_TRUE(t1->has_descendant(t3));
	 
	 std::set<const SequenceOntology::Term*> L;
	 t1->collect(L);
	 ASSERT_TRUE(L.find(t1)!=L.end());
	 ASSERT_TRUE(L.find(t3)!=L.end());
	}


int main_tests(int argc, char**argv) {
	TestsArgs args;
	if(!args.parse(argc,argv)) return EXIT_FAILURE;
	PROGRAM_COMMON(args);
	string directory(args.datadirectory);
	try {
		test_bed_01(directory);
		test_samviewwithmate_01(directory);
		test_sequence_ontology();
		cerr << "Done" << endl;
		}
	catch(exception& err) {
	cerr << "ERROR " << err.what() << endl;
		return -1;
		}
	return 0;
	}
