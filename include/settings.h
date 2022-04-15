// Copyright [2022] <Thomas Caleb, Matteo Losacco>

#ifndef DEF_SETTINGS
#define DEF_SETTINGS

#pragma once

// Define usiful constants
#define PI (4.0*atan(1.0)) // definiton of pi [rad]
#define SIZE_VECTOR 6 // Size of a state vector (3 positions, 3 velocities)
#define SIZE_VECTOR_PL 4 // Size of a state vector (2 positions, 2 velocities)
#define SEC2DAYS (1.0/(24.*3600)) // Conversion from seconds to days [days.s^-1]

// Define dynamical system
#define MU_1 (3.9860043543609598e5) // Mass parameter of the first primary [km^3.s^-2]
#define MU_2 (4.9028000661637961e3) // Mass parameter of the second primary [km^3.s^-2]
#define SMA (384399) // Distance between the two primaries [km]

// Define normalization units
#define MU (MU_2 / (MU_1 + MU_2)) // Mass ratio of the primaries [-]
#define LU SMA // Lenght unit [km]
#define WU (sqrt((MU_1 + MU_2)/pow(SMA, 3))) // Pulsation unit [rad.s^-1]
#define TU (1/WU) // Time unit [s]
#define VU sqrt(LU/TU) // Velecity unit [km.s^-1]

// Save/Load paths
#define SAVE_PATH_FAMILY "./data/family/" // Directory where to save Families

#endif
