// Copyright [2022] <Thomas Caleb, Matteo Losacco>

#include "family.h"

using namespace std;
using namespace DACE;

// Constructors
Family::Family() :
	name_(""), list_map_(vector<vectorDA>(0)),
	list_interval_(vector<vector<vectordb>>(0)),
	description_(""), interval_(vector<vectordb>(0)) {}
Family::Family(
	string const& name,
	vector<vectorDA> const& list_map,
	vector<vector<vectordb>> const& list_interval,
	string const& description) :
	name_(name), list_map_(list_map),
	list_interval_(list_interval), description_(description) {

	// Compute interval
	vector<vectordb> interval(2);
	interval[0] = list_interval[0][0];
	interval[1] = list_interval[list_interval.size() - 1][2];
	interval_ = interval;
}

// Copy constructor
Family::Family(Family const& family) :
	name_(family.name_), list_map_(family.list_map_),
	list_interval_(family.list_interval_), interval_(family.interval_),
	description_(family.description_) {}

// Load constructor
Family::Family(string const& file_name) {
	// Open file
	string file_name_(SAVE_PATH_FAMILY + file_name + ".dat");
	ifstream ifs(file_name_);

	// Load data
	ifs >> *this;
	ifs.close();
	return;
}

// Destructor
Family::~Family() {}

// Getters
const string Family::name() const { return name_; }
const vector<vectorDA> Family::list_map() const { return list_map_; }
const vector<vector<vectordb>> Family::list_interval() const { return list_interval_; }
const vector<vectordb> Family::interval() const { return interval_; }
const string Family::description() const { return description_; }

// Interval manipulaiton

// True if the parameter belongs the interval
const bool Family::in_interval(vectordb const& parameter) const {

	// Check size
	if (parameter.size() != interval_[0].size()) {
		cout << "Error : Input parameter is not of a compatible size : "
			<< parameter.size() << ", versus size : " << interval_[0].size() << endl;
		return false;
	}

	// Check parameter belongs
	for (size_t i = 0; i < interval_[0].size(); i++) {
		if (!(parameter[i] >= interval_[0][i]
			&& parameter[i] <= interval_[1][i])) {
			return false;
		}
	}
	return true;
}

// True if the parameter belongs an interval
const bool Family::in_interval(
	vectordb const& parameter,
	size_t const& index) const {

	// Check size
	if (parameter.size() != interval_[0].size()) {
		cout << "Error : Input parameter is not of a compatible size : "
			<< parameter.size() << ", versus size : " << interval_[0].size() << endl;
		return false;
	}
	// Check index
	if (index >= list_interval_.size()) {
		cout << "Error : Input index is larger that list_interval size : "
			<< index << ", versus size : " << list_interval_.size() << endl;
		return false;
	}

	// Check parameter belongs
	for (size_t i = 0; i < interval_[0].size(); i++) {
		if (!(parameter[i] >= list_interval_[index][0][i]
			&& parameter[i] <= list_interval_[index][2][i])) {
			return false;
		}
	}
	return true;
}

// Returns the map that will be evaluated with the parameter
const size_t Family::evaluation_index(vectordb const& parameter) const {
	// Check belonging
	if (!in_interval(parameter)) {
		cout << "Error : The parameter does not belong the interval of validity of the family" << endl;
		return parameter.size();
	}

	// Dichotomy
	size_t nb_map = list_map_.size();
	size_t nb_component = interval_[0].size();
	size_t index = nb_map / 2;
	size_t bound_low = 0; size_t bound_up = nb_map;
	bool low = true; bool high = false;
	vectordb vector_low, vector_high;
	vector<vectordb> interval_index;
	while (low || high) {
		interval_index = list_interval_[index];

		// Compute diff
		vector_low = parameter - interval_index[0];
		vector_high = interval_index[2] - parameter;

		// Check condition
		low = false; high = false;
		for (size_t i = 0; i < nb_component; i++) {
			if (vector_low[i] < 0) {
				low = true;	break;
			}
			if (vector_high[i] < 0) {
				high = true; break;
			}
		}

		// Change index
		if (low) {
			bound_up = index;
			index = (bound_up + bound_low) / 2;
		}
		if (high) {
			bound_low = index;
			index = (bound_up + bound_low) / 2;
		}
	}

	// Check result
	if (in_interval(parameter, index)) {
		return index;
	}

	// If a interval could not be found
	cout << "Error : The parameter does not belong the interval of validity of the family"
		<< endl << "Verify the construction of the family" << endl;
	return parameter.size();
}

// DA related methods
const vectordb Family::eval(vectordb const& parameter) const {
	// Get evaluation index and check belonging
	size_t index = evaluation_index(parameter);
	if (index == list_interval_.size()) {
		return vectordb(0);
	}

	// Center
	vectordb parameter_ = parameter - list_interval_[index][1];

	// Normalize
	for (size_t i = 0; i < interval_[0].size(); i++) {
		parameter_[i] /= list_interval_[index][2][i] - list_interval_[index][1][i];
	}

	// Evaluate and return
	return list_map_[index].eval(parameter_);
}
const vector<vectordb> Family::eval(vector<vectordb> const& list_parameter) const {
	size_t size_list = list_parameter.size();
	vector<vectordb> output(size_list);

	// Init list index
	vector<vector<size_t>> list_index(list_map_.size(), vector<size_t>(0));
	for (size_t i = 0; i < size_list; i++) {
		list_index[evaluation_index(list_parameter[i])].push_back(i);
	}

	// Run through list index
	for (size_t i = 0; i < list_index.size(); i++) {

		// Unpack
		vector<size_t> list_index_map = list_index[i];
		vector<vectordb> interval = list_interval_[i];
		vectordb center = interval[1];
		vectordb norm = interval[2] - interval[1];
		compiledDA map = list_map_[i].compile();
		for (size_t j = 0; j < list_index_map.size(); j++) {
			// Get index
			size_t index = list_index_map[j];

			// Center
			vectordb parameter = list_parameter[index] - center;

			// Normalize
			for (size_t k = 0; k < norm.size(); k++) {
				parameter[k] /= norm[k];
			}

			// Evaluate and assign
			output[index] = map.eval(parameter);
		}
	}

	return output;
}

// Save/Load
void Family::save(string const& file_name) const {
	// Open file
	string file_name_(SAVE_PATH_FAMILY + file_name + ".dat");
	ofstream ofs(file_name_);

	// Store the object to file
	ofs << *this;

	ofs.close();
	return;
}
void Family::load(string const& file_name) {
	// Open file
	string file_name_(SAVE_PATH_FAMILY + file_name + ".dat");
	ifstream ifs(file_name_);

	// Load data
	ifs >> *this;

	ifs.close();
	return;
}

// IO operator
ostream& operator<<(ostream& os, const Family& f) {
	// Set double precision
	typedef std::numeric_limits<double> dbl;
	os.precision(dbl::max_digits10);

	// Get size of lists
	size_t size_list = f.list_interval_.size();

	// Writing name and description on 2 lines
	os << f.name_ << endl;
	os << f.description_ << endl;

	// DA data
	os << DA::getMaxOrder() << endl;
	os << DA::getMaxVariables() << endl;

	// Number of maps
	os << size_list << endl;

	// Interval
	os << f.interval_[0];
	os << f.interval_[1];

	// List interval
	for (size_t i = 0; i < size_list; i++) {
		os << f.list_interval_[i][0];
		os << f.list_interval_[i][1];
		os << f.list_interval_[i][2];
	}

	// List map
	for (size_t i = 0; i < size_list; i++) {
		os << f.list_map_[i];
	}

	return os;
}
istream& operator>>(istream& is, Family& f) {
	size_t size_list = 0;

	// Reading simple property from a line
	getline(is, f.name_);
	getline(is, f.description_);

	// DA data
	string order_str, nb_variable_str;
	unsigned int order, nb_variable;
	getline(is, order_str); getline(is, nb_variable_str);
	istringstream(order_str) >> order; istringstream(nb_variable_str) >> nb_variable;

	// Init DACE if needed
	if (!DA::isInitialized()) {
		DA::init(order, nb_variable);
		DA::setEps(1e-90);
	}

	// Number of maps
	string size_str;
	getline(is, size_str);
	istringstream(size_str) >> size_list;

	// Interval
	vector<vectordb> interval(2);
	is >> interval[0];
	is >> interval[1];

	f.interval_ = interval;

	// List interval
	vector<vector<vectordb>> list_interval(size_list);
	vector<vectordb> interval_buff(3);
	for (size_t i = 0; i < size_list; i++) {
		is >> interval_buff[0];
		is >> interval_buff[1];
		is >> interval_buff[2];
		list_interval[i] = interval_buff;
	}
	f.list_interval_ = list_interval;

	// List Map
	vector<vectorDA> list_map(size_list);
	for (size_t i = 0; i < size_list; i++) {
		is >> list_map[i];
	}
	f.list_map_ = list_map;

	return is;
}

// Operators
Family& Family::operator=(const Family& f) {
	// No self assignment
	if (this == &f) {
		return *this;
	}

	name_ = f.name_;
	list_map_ = f.list_map_;
	list_interval_ = f.list_interval_;
	interval_ = f.interval_;
	description_ = f.description_;
	return *this;
}
