// Copyright [2022] <Thomas Caleb, Matteo Losacco>

#ifndef DEF_EOM
#define DEF_EOM

#pragma once

#include <dace/dace_s.h>

#include "settings.h"
#include "integration.h"

// Without STM
// I/O state vector is 1X7 (6 + time)
template<typename T>
DACE::AlgebraicVector<T>  acceleration_7(
    DACE::AlgebraicVector<T>  const& state_vector,
    double const& t) {
    // Unpack
    T x = state_vector[0];
    T y = state_vector[1];
    T z = state_vector[2];
    T x_p = state_vector[3];
    T y_p = state_vector[4];
    T z_p = state_vector[5];
    T s = state_vector[6]; // Period

    // velocity
    DACE::AlgebraicVector<T>  output(SIZE_VECTOR + 1);
    output[0] = x_p;
    output[1] = y_p;
    output[2] = z_p;
    output[6] = 0.0; // Period is constant

    // distances
    double mu = MU;
    T r_1 = sqrt(pow(x + mu, 2) + pow(y, 2) + pow(z, 2));
    T r_2 = sqrt(pow(x - (1 - mu), 2) + pow(y, 2) + pow(z, 2));

    // Acceleration
    output[3] = (x + 2 * y_p
        - (1 - mu) * (x + mu) / pow(r_1, 3)
        - mu * (x - (1 - mu)) / pow(r_2, 3));
    output[4] = (y - 2 * x_p
        - (1 - mu) * y / pow(r_1, 3)
        - mu * y / pow(r_2, 3));
    output[5] = (-z * ((1 - mu) / pow(r_1, 3) + mu / pow(r_2, 3)));

    return s*output;
}

// Without STM
// I/O state vector is 1X8 (6 + time + phase)
template<typename T>
DACE::AlgebraicVector<T>  acceleration_8(
    DACE::AlgebraicVector<T>  const& state_vector,
    double const& t) {
    // Unpack
    T x = state_vector[0];
    T y = state_vector[1];
    T z = state_vector[2];
    T x_p = state_vector[3];
    T y_p = state_vector[4];
    T z_p = state_vector[5];
    T s = state_vector[6]; // Period
    T phi = state_vector[7]; // Phase

    // velocity
    DACE::AlgebraicVector<T>  output(SIZE_VECTOR + 2);
    output[0] = s * x_p;
    output[1] = s * y_p;
    output[2] = s * z_p;
    output[6] = 0.0; // Period is constant
    output[7] = 2*PI; // Phase is linear

    // distances
    double mu = MU;
    T r_1 = sqrt(pow(x + mu, 2) + pow(y, 2) + pow(z, 2));
    T r_2 = sqrt(pow(x - (1 - mu), 2) + pow(y, 2) + pow(z, 2));

    // Acceleration
    output[3] = s * (x + 2 * y_p
        - (1 - mu) * (x + mu) / pow(r_1, 3)
        - mu * (x - (1 - mu)) / pow(r_2, 3));
    output[4] = s * (y - 2 * x_p
        - (1 - mu) * y / pow(r_1, 3)
        - mu * y / pow(r_2, 3));
    output[5] = s * (-z * ((1 - mu) / pow(r_1, 3) + mu / pow(r_2, 3)));

    return output;
}

// Returns the value of the constraint on the halo for a 1 x 7 state vector
template<typename T>
DACE::AlgebraicVector<T> constraint(DACE::AlgebraicVector<T> const& x_0) {
    DACE::AlgebraicVector<T> x_f = RK78(SIZE_VECTOR + 1,
        x_0.extract(0, SIZE_VECTOR), 0, 0.5, acceleration_7);
    DACE::AlgebraicVector<T> output(3);
    output[0] = x_f[1]; output[1] = x_f[3]; output[2] = x_f[5];
    return output;
}
// Returns the value of the constraint on the halo for a 1 x 8 state vector
DACE::AlgebraicVector<double> constraint_8(DACE::AlgebraicVector<double> const& x_0);

#endif
