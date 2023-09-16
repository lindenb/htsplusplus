#include "debug.hh"
#include "RegIdx.hh"

using namespace std;


std::unique_ptr<RegIdx> RegIdx::load_file(const char* in_fname) {
	if(in_fname==NULL) {
		THROW_ERROR("filename is NULL");
		}
	RegIdx* ptr = new RegIdx;
	ptr->idx = ::regidx_init(in_fname, NULL , NULL ,0, NULL);
	if(ptr->idx==NULL) {
		delete ptr;
		THROW_ERROR("cannot load " << in_fname);
		}
	return std::unique_ptr<RegIdx>(ptr);
	}
