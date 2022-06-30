// Copyright [2022] <Thomas Caleb, Matteo Losacco>

#include "example.h"

using namespace DACE;
using namespace std;
using namespace std::chrono;

int example(int argc, char** argv) {

	// Input check
	if (argc < 2) {
		cout << "Wrong number of arguments." << endl;
		cout << "Requested number : 1 (+0)." << endl;
		cout << "0 - Example to execute (1 or 2)." << endl;
		return 0;
	}
	
	// Unpack inputs
	unsigned int script = atoi(argv[1]);

	// Execute script
	switch (script) {
	case 1:
		return example_1(argc, argv);
	case 2:
		return example_2(argc, argv);
	default:
		break;
	}

	return 0;
}

int main(int argc, char** argv) {
	return example(argc, argv);
}

