/*
 * LlaEcefConverter.hpp
 *
 *  Created on: December 1, 2025
 *      Author: Lane Hale
 *
 * Description: Header file containing constants, structures, and function 
 *              declarations for converting Latitude/Longitude/Altitude (LLA) 
 *              to Earth-Centered Earth-Fixed (ECEF) coordinates, including 
 *              velocity interpolation at arbitrary times.
 *
 * Purpose: Personal coding challenge demonstrating precision math, 
 *          coordinate transformations, and modern C++ practices.
 */

#pragma once

#include <format>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <libInterpolate/Interpolate.hpp>
#include <libInterpolate/Interpolators/_1D/LinearInterpolator.hpp>

// Define a type that uses 50 decimal places of precision
using HighPrecisionFloat = boost::multiprecision::cpp_dec_float_50;
// Helps the compiler find operators
using namespace boost::multiprecision;
// Define a linear interpolator for high precision doubles
using VelocityInterpolator = _1D::LinearInterpolator<HighPrecisionFloat>;

// File path to the CSV data file
const std::string kFILEPATH{ "sample_lla_data.csv" };
// WGS84 Constants (using high precision)
const HighPrecisionFloat kA = 6378137.0;            // Semi-major axis (meters)
const HighPrecisionFloat kF = 1.0 / 298.257223563;  // Flattening
const HighPrecisionFloat kE_SQ = kF * (2.0 - kF);   // Eccentricity squared
// Pi constant with high precision to 50 decimal places
const HighPrecisionFloat kPI_HP("3.14159265358979323846264338327950288419716939937510");
const HighPrecisionFloat kDEG_TO_RAD = kPI_HP / 180.0;
const HighPrecisionFloat kKM_TO_M = 1000.0;

// Latitude, Longitude, Altitude (LLA) Geodetic Coordinates
struct LlaData
{
	HighPrecisionFloat time;
	HighPrecisionFloat latitude;
	HighPrecisionFloat longitude;
	HighPrecisionFloat altitude;
};

// Earth-Centered, Earth-Fixed (ECEF) Geocentric Coordinates
struct EcefData
{
	HighPrecisionFloat time;
	HighPrecisionFloat x;    // East-West axis through Greenwich meridian
	HighPrecisionFloat y;    // North-South axis
	HighPrecisionFloat z;    // Axis through the North Pole
};

// Function to Get LLA Data from a CSV file
std::vector<LlaData> getLlaData(const std::string& filename);

// Function to Convert LLA coordinates to ECEF coordinates
std::vector<EcefData> convertLlaToEcef(const std::vector<LlaData>& lla_data);

// Function to Calculate Velocity between ECEF coordinates
void calculateDiscreteVelocities(
	const std::vector<EcefData>& ecef_data,
	std::vector<HighPrecisionFloat>& velocity_time_vector,
	std::vector<HighPrecisionFloat>& velocity_x_vector,
	std::vector<HighPrecisionFloat>& velocity_y_vector,
	std::vector<HighPrecisionFloat>& velocity_z_vector
);

// Function to Get the Interpolated Velocity at a specific time
void getInterpolatedVelocity(
	HighPrecisionFloat time_to_find,
	const VelocityInterpolator& x_velocity_interpolator,
	const VelocityInterpolator& y_velocity_interpolator,
	const VelocityInterpolator& z_velocity_interpolator
);
