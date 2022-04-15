// Copyright [2022] <Thomas Caleb, Matteo Losacco>

#ifndef DEF_FAMILY
#define DEF_FAMILY

#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <limits>

#include <dace/dace_s.h>

#include "settings.h"

class Family {

// Attributes
private:
	std::string name_; // name of the family
	std::vector<DACE::vectorDA> list_map_; // List of the vectorDA
	std::vector<std::vector<DACE::vectordb>> list_interval_; // Interval of validity for each vectorDA
	std::vector<DACE::vectordb> interval_; // Interval of validity for whole family
	std::string description_; // Text description of the mapped family

// Methods
public:

	// Constructors
	Family();
	Family(
		std::string const& name,
		std::vector<DACE::vectorDA> const& list_map,
		std::vector<std::vector<DACE::vectordb>> const& list_interval,
		std::string const& description);
	Family(Family const& family); // Copy constructor
	Family(std::string const& file_name); // Load constructor

	// Destructors
	~Family();

	// Getters
	const std::string name() const;
	const std::vector<DACE::vectorDA> list_map() const;
	const std::vector<std::vector<DACE::vectordb>> list_interval() const;
	const std::vector<DACE::vectordb> interval() const;
	const std::string description() const;

	// Interval manipulaiton
	const bool in_interval(DACE::vectordb const& parameter) const; // True if the parameter belongs the interval
	const bool in_interval(
		DACE::vectordb const& parameter,
		std::size_t const& index) const; // True if the parameter belongs an interval
	const size_t evaluation_index(DACE::vectordb  const& parameter) const; // Returns the map that will be evaluated with the parameter

	// DA related methods
	const DACE::vectordb eval(DACE::vectordb const& parameter) const;
	const std::vector<DACE::vectordb> eval(std::vector<DACE::vectordb> const& list_parameter) const;

	// Save/Load
	void save(std::string const& file_name) const;
	void load(std::string const& file_name);

	// IO operator
	friend std::ostream& operator<<(std::ostream& os, const Family& f);
	friend std::istream& operator>>(std::istream& is, Family& f);

	// Operators
	Family& operator=(const Family& f);
};

#endif