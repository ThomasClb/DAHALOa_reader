// Copyright [2022] <Thomas Caleb, Matteo Losacco>

#include "example.h"

using namespace DACE;
using namespace std;
using namespace std::chrono;

int example_1(int argc, char** argv) {

	// Input check
	if (argc < 3) {
		cout << "Wrong number of arguments." << endl;
		cout << "Requested number : 3." << endl;
		cout << "0 - Example to execute (here 2)." << endl;
		cout << "1 - Name of the family to read in the './data/family/' directory (ex 'DRO/x_phi_8_5')." << endl;
		return 0;
	}

	// Unpack arameters
	string name_file = argv[2]; // name of the file in the "./data/family" directory

	// Load family
	Family family(name_file); // Load the file into the family object

	// Output
	cout << "Family : " + family.name() + " loaded" << endl;

	// Properties of the family object
	cout << endl << endl;
	cout << "---------------------- BASIC PROPERTIES ----------------------" << endl;

	// Name
	cout << "Name : " << family.name() << endl;

	// Decription
	cout << "Description : " << family.description() << endl;

	// Get the interval of definition of the interval [lower_bound, upper_bound]
	// Any evaluation attemps below of above this interval will fail
	vector<vectordb> interval = family.interval(); 
	cout << "Interval : " << endl;
	cout << "Lower bound [first variable, second variable] : " << endl;
	cout << interval[0] << endl;
	cout << "Upper bound [first variable, second variable] : " << endl;
	cout << interval[1] << endl;

	// Select one index
	size_t i = 42;

	// Get the i-th map and associated interval
	vectorDA map_i = family.list_map()[i]; // i-th element of the vector of all maps (vectorDA objects)
	vector<vectordb> interval_i = family.list_interval()[i]; // i-th element of the vector of all intervals (vector<vectordb> objects) [lower_bound, middle_point, upper_bound]

	// Interval of definition of the map
	cout << "Lower bound [first variable, second variable] : " << endl;
	cout << interval_i[0] << endl;
	cout << "Upper bound [first variable, second variable] : " << endl;
	cout << interval_i[2] << endl; // !! The upper bound is the third element of the vector

	// Expansion point of the map
	cout << "Expansion point of the polynimal map [first variable, second variable] : " << endl;
	cout << interval_i[1] << endl; // The expansion point is the second element of the vector

	// Show the second component of the map (it is long)
	// It is a polynomial of 2 variables
	cout << "Second component of the map : " << endl;
	cout << map_i[1] << endl;

	// Interval manipulations
	cout << endl << endl;
	cout << "---------------------- INTERVALS ----------------------" << endl;

	// Check if a point belongs to the interval
	vectordb point = (interval[0] + interval[1]) / 2; // Create a point in the middle of the interval (inside)
	cout << "Point to evaluate [first variable, second variable] : " << endl;
	cout << point << endl;

	// Check if the interval contains this point
	bool in_interval = family.in_interval(point); 
	cout << "Does this point belong to the interval ? : " << in_interval << endl;

	// Check which map represents this part of the domain
	size_t evaluation_index = family.evaluation_index(point);
	cout << "The point is represented by the " << evaluation_index << "-th map" << endl;

	// Check (again) if this index-th map represents this point
	in_interval = family.in_interval(point, evaluation_index);
	cout << "Are you sure this point belong to the " << evaluation_index << "-th interval ? : " << in_interval << endl;

	// Same with a bad point (way too far)
	vectordb bad_point = point * 1e12;
	cout << "Bad point to evaluate [first variable, second variable] : " << endl;
	cout << bad_point << endl;

	// Check if the interval contains this point
	in_interval = family.in_interval(bad_point);
	cout << "Does this point belong to the interval ? : " << in_interval << endl;

	// Check which map represents this part of the domain
	evaluation_index = family.evaluation_index(bad_point);
	cout << "The point is represented by the " << evaluation_index << "-th map" << endl;

	// Check (again) if this index-th map represents this point
	in_interval = family.in_interval(bad_point, evaluation_index);
	cout << "Are you sure this point belong to the " << evaluation_index << "-th interval ? : " << in_interval << endl;


	// Evaluation
	cout << endl << endl;
	cout << "---------------------- EVALUATION ----------------------" << endl;

	// Evaluation of one point
	vectordb evaluation = family.eval(point); // Call to evaluation function
	cout << "Evaluation of one single point : " << endl;
	cout << "Evaluation of : " << endl;
	cout << point << endl;
	cout << "Delivers : " << endl;
	cout << evaluation << endl;

	// Evaluation of several points at once
	cout << "Evaluation of several points at once : " << endl;

	// Build list of point
	vector<vectordb> list_point{ interval[0] + point / 2, point, interval[1] - point/2 }; // first quarter, middle and last quarter of the interval
	vector<vectordb> list_evaluation = family.eval(list_point); // Call to evaluation function
	cout << "Evaluation of " << list_point.size() << " points" << endl;
	cout << "First point evaluated : " << endl;
	cout << list_point[0] << endl;
	cout << "Delivers : " << endl;
	cout << list_evaluation[0] << endl;

	// With unormalized units
	evaluation = list_evaluation[0];
	cout << "First results with unormalized units in the CR3BP rotating frame centered on the barycenter, and mu = " << MU << endl;
	cout <<  "x [km], y [km], z [km], v_x [km/s], v_y [km/s], v_z [km/s], T [s]" << endl;
	cout << evaluation[0] * LU << ", " << evaluation[1] * LU << ", " << evaluation[2] * LU << ", "
		<< evaluation[3] * VU << ", " << evaluation[4] * VU << ", " << evaluation[5] * VU << ", "
		<< evaluation[6] * TU << endl;

	return 0;
}
