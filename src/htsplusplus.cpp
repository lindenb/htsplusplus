#include "programs.hh"



int main(int argc,char** argv) {
	MainArgs app;
	try {
		return app.main(argc, argv);
		}
	catch(...) {
		std::cerr << "Error Raised. Exiting." << std::endl;
		return EXIT_FAILURE;
		}
	}
