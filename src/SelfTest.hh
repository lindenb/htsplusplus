#ifndef SELF_TEST_HH
#define SELF_TEST_HH
#include "Program.hh"

namespace htspp {

class SelfTest : public Program {
	private:
			long n_passing;
			long n_fail;
	public:
	        std::string base;
			SelfTest();
			virtual ~SelfTest();
			void check(const char* fname,int line,bool b);
			virtual int main(int argc,char** argv);
	};


}

#endif
