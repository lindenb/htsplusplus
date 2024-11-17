#ifndef SELF_TEST_HH
#define SELF_TEST_HH

namespace htspp {
class SelfTest {
	private:
			long n_passing;
			long n_fail;
			
	public:
			SelfTest();
			void check(const char* fname,int line,bool b);
			int run(const char* base);
	};


}

#endif
