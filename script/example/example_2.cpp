// Copyright [2022] <Thomas Caleb, Matteo Losacco>

#include "example.h"

using namespace DACE;
using namespace std;
using namespace std::chrono;

vector<vectordb> error_grid(
	Family const& family,
	vector<vectordb> const& list_eval) {

	// Evaluate contraintes
	vector<vectordb> output(list_eval.size());
	for (size_t i = 0; i < list_eval.size(); i++) {
		output[i] = constraint_8(list_eval[i]); // Evaluate constraints
	}

	return output;
}

vector<vectordb> get_grid(
	Family const& family,
	unsigned int const& size_grid) {
	vector<vectordb> interval = family.interval(); // Interval of definition of the map [lower_bound, upper_bound]
	double bound_0_min = interval[0][0];
	double bound_0_max = interval[1][0];

	// Build a 2D grid on the interval of definition of the interval for evaluation
	vector<vectordb> grid(0);
	for (size_t i = 0; i < size_grid; i++) {
		for (size_t j = 0; j < size_grid; j++) {
			vectordb parameter = interval[0];
			parameter[0] = bound_0_min;
			parameter[0] += i * (bound_0_max - bound_0_min) / (size_grid - 1.0);
			parameter[1] = 0;
			parameter[1] += j * (2 * PI) / (size_grid - 1.0);
			grid.push_back(parameter);
		}
	}
	return grid;
}

int example_2(int argc, char** argv) {

	// Input check
	if (argc < 4) {
		cout << "Wrong number of arguments." << endl;
		cout << "Requested number : 3." << endl;
		cout << "0 - Example to execute (here 2)." << endl;
		cout << "1 - Name of the family to read in the './data/family/' directory (ex 'DRO/x_phi_8_5')." << endl;
		cout << "2 - Size of the grid to evaluate, recommanded 50." << endl;
		return 0;
	}

	// Unpack arameters
	string name_file = argv[2]; // name of the file in the "./data/family" directory
	int size_grid = atoi(argv[3]); // Size of the grid to evaluate : size_grid x size_grid

	// Load family
	auto start = high_resolution_clock::now();
	Family family(name_file); // Load the file into the family object
	auto stop = high_resolution_clock::now();
	auto duration_mapping = duration_cast<microseconds>(stop - start);

	// Output
	cout << "Family : " + family.name() + " loaded in " + to_string(static_cast<int>(duration_mapping.count()) / 1e6) + "s" << endl;
	
	// Get grid
	size_t nb_map = family.list_map().size(); // Number of polynomial used to describe the interval
	vector<vectordb> grid = get_grid(family, size_grid); // Build a 2D grid over the interval

	// Evaluate maps
	start = high_resolution_clock::now();
	vector<vectordb> list_eval = family.eval(grid); // Evaluation of a list of parameters of size size_grid x size_grid
	stop = high_resolution_clock::now();
	auto duration_evaluation = duration_cast<microseconds>(stop - start);

	// Output
	cout << "Evaluation of " << size_grid*size_grid << " points in " + to_string(static_cast<int>(duration_evaluation.count()) / 1e6) + "s" << endl;

	// Compute error
	start = high_resolution_clock::now();
	vector<vectordb> list_contraint = error_grid(family, list_eval); // Evaluate the constraint violation for size_grid x size_grid initial condition
	stop = high_resolution_clock::now();
	auto duration_contraint = duration_cast<microseconds>(stop - start);

	// Output
	cout << "Contraint violation evaluated in " + to_string(static_cast<int>(duration_contraint.count()) / 1e6) + "s" << endl;

	// Benchmark data
	double mean(0.0), max(1e-30);
	for (size_t i = 0; i < list_contraint.size(); i++) {
		double norm = list_contraint[i].vnorm(); // Compute norm of constraint violation vector
		if (norm > max)
			max = norm; // Compute max of constraint violations
		mean += norm; // Compute mean of constraint violations
	}
	mean /= list_contraint.size();

	// Output
	cout << "Contraint violation : " << endl;
	cout << "- Max constraint violation [-] : " << max << endl;
	cout << "- Mean constraint violation [-] : " << mean << endl;

	return 0;
}
