// Copyright [2022] <Thomas Caleb, Matteo Losacco>

#include "eom.h"

using namespace std;
using namespace DACE;

// Returns the value of the constraint on the halo for a 1 x 8 state vector
AlgebraicVector<double> constraint_8(AlgebraicVector<double> const& x_0) {
    AlgebraicVector<double> output(3);

    double phi = x_0[7];
    double T = x_0[6];
    double start_time, end_time;
    if (phi >= 0.0 && phi <= PI / 2) {
        start_time = phi / 2 / PI;
        end_time = 0.5;
    }
    if (phi >= PI / 2 && phi <= PI) {
        start_time = phi / 2 / PI;
        end_time = 0.0;
    }
    if (phi >= PI && phi <= 3 * PI / 2) {
        start_time = phi / 2 / PI;
        end_time = 1;
    }
    if (phi >= 3 * PI / 2 && phi <= 2 * PI) {
        start_time = phi / 2 / PI;
        end_time = 0.5;
    }

    // Propagation
    AlgebraicVector<double> x_f = RK78(SIZE_VECTOR + 1,
        x_0.extract(0, SIZE_VECTOR), start_time, end_time, acceleration_7);
    output[0] = x_f[1]; output[1] = x_f[3]; output[2] = x_f[5];

    return output;
}