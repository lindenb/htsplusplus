#include "SamFileReader.hh"
#include "programs.hh"
using namespace std;

int main_samviewwithmate(int argc, char** argv) {
	SamviewwithmateArgs args;
	args.parse(argc,argv);
	if(args.show_help) {
		args.usage(cout);
		return EXIT_SUCCESS;
		}

	SamFileReaderFactory srf;
	std::unique_ptr<SamFileReader> r=srf.open("-");
	SamRecord rec;
	int n=0;
	while(r->read2(&rec)) {
	n++;
		}
	DEBUG(n);

	return 0;
	}
