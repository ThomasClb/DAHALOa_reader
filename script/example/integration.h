// Copyright [2022] <Thomas Caleb, Matteo Losacco>

#ifndef DEF_INTEGRATION
#define DEF_INTEGRATION

#pragma once

#include <cmath>

#include <dace/dace_s.h>

#include "settings.h"

// Integration constants
#define MIN_STEP 1e-20 // Minimum step size [TU]
#define EPS 4e-12 // Tolerance of the intergration scheme [-]

// Declare integration matrices
const std::vector<double> get_A();
const std::vector<double> A = get_A();
const std::vector<double> get_C();
const std::vector<double> C = get_C();
const std::vector<double> get_D();
const std::vector<double> D = get_D();
const DACE::AlgebraicMatrix<double> get_B();
const DACE::AlgebraicMatrix<double> B = get_B();

// Utilities
double min(double const& a, double const& b);
double max(double const& a, double const& b);
double normtmp(int const& N, std::vector<double> const& X);

// Classic integrators
DACE::AlgebraicVector<DACE::DA> RK78(
    int const& N, DACE::AlgebraicVector<DACE::DA> Y0,
    double const& X0, double const& X1,
    DACE::AlgebraicVector<DACE::DA>(*f)(DACE::AlgebraicVector<DACE::DA> const&, double const&));
DACE::AlgebraicVector<double> RK78(
    int const& N, DACE::AlgebraicVector<double> Y0,
    double const& X0, double const& X1,
    DACE::AlgebraicVector<double>(*f)(DACE::AlgebraicVector<double> const&, double const&));

#endif
