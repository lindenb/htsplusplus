#include <stdexcept>
#include "programs.hh"

using namespace std;

int main(int argc,char** argv) {
	MainArgs app;
	try {
		return app.main(argc, argv);
		}
	catch(exception& err) {
		std::cerr << "Error: " << err.what() << std::endl;
		return EXIT_FAILURE;
		}
	catch(...) {
		std::cerr << "Error: Exiting." << std::endl;
		return EXIT_FAILURE;
		}
	}
