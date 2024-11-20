#include "Program.hh"

using namespace htspp;
using namespace std;


Program::Program(const char* name,const char* description):_name(name),_description(description) {
}
Program::~Program() {
 }
 
bool Program::is_hidden() const {
    return false;
    }
 
const char* Program::name() const { return _name.c_str();}
const char* Program::description() const { return _description.c_str();}


void Program::usage(std::ostream& out,GetOpt* getopt) {
	out << _name << endl;
	out << "Description: " << _description << endl;
	if(getopt!=NULL) {
		getopt->usage(out);
		}
	out << endl;
	}