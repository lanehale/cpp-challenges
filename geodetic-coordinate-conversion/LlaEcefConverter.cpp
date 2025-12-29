/*
 * LlaEcefConverter.cpp
 *
 *  Created on: December 1, 2025
 *      Author: Lane Hale
 *
 * Description: This file contains the main program logic, LLA to ECEF coordinate
 *              conversions, velocity calculations, and linear interpolation to
 *              get velocities at arbitrary times.
 *
 *     Purpose: Personal coding challenge demonstrating precision math, 
 *              coordinate transformations, and modern C++ practices.
 */

 #include "LlaEcefConverter.hpp"

// ---------------------------------------- //
// Function to Get LLA Data from a CSV file //
// ---------------------------------------- //
std::vector<LlaData> getLlaData(const std::string& filename) {
    std::vector<LlaData> lla_output_vector;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        LlaData current_lla_point;
        std::string temp_comma;

        ss >> current_lla_point.time;  // Stream extraction (>>) skips white space
        ss >> temp_comma;  // Read and discard comma

        ss >> current_lla_point.latitude;
        ss >> temp_comma;

        ss >> current_lla_point.longitude;
        ss >> temp_comma;

        ss >> current_lla_point.altitude;
        current_lla_point.altitude *= 1000.0;  // Convert to meters

        // Add the LLA data to the output vector
        lla_output_vector.push_back(current_lla_point);
    }

    file.close();
    return lla_output_vector;
}

// ------------------------------------------------------- //
// Function to Convert LLA coordinates to ECEF coordinates //
// ------------------------------------------------------- //
std::vector<EcefData> convertLlaToEcef(const std::vector<LlaData>& lla_data_input) {
    std::vector<EcefData> ecef_output_vector;

    for (size_t i = 0; i < lla_data_input.size(); ++i) {
        EcefData current_ecef_point;
        HighPrecisionFloat height_meters = lla_data_input[i].altitude;

        // Convert LLA degress to radians
        HighPrecisionFloat latitude_degrees = lla_data_input[i].latitude;
        HighPrecisionFloat longitude_degrees = lla_data_input[i].longitude;
        HighPrecisionFloat latitude_radians = latitude_degrees * kDEG_TO_RAD;
        HighPrecisionFloat longitude_radians = longitude_degrees * kDEG_TO_RAD;

        // Calculate N (Radius of Curvature in the Prime Vertical)
        HighPrecisionFloat sine_latitude = sin(latitude_radians);
        HighPrecisionFloat radius_of_curvature_n = kA / sqrt(1.0 - kE_SQ * sine_latitude * sine_latitude);

        // Calculate ECEF coordinates
        HighPrecisionFloat cosine_latitude = cos(latitude_radians);
        HighPrecisionFloat cosine_longitude = cos(longitude_radians);
        HighPrecisionFloat sine_longitude = sin(longitude_radians);

        current_ecef_point.x = (radius_of_curvature_n + height_meters) * cosine_latitude * cosine_longitude;
        current_ecef_point.y = (radius_of_curvature_n + height_meters) * cosine_latitude * sine_longitude;
        current_ecef_point.z = ((1.0 - kE_SQ) * radius_of_curvature_n + height_meters) * sine_latitude;
        current_ecef_point.time = lla_data_input[i].time;

        // Add the converted ECEF data to the output vector
        ecef_output_vector.push_back(current_ecef_point);
    }

    return ecef_output_vector;
}

// ------------------------------------------------------- //
// Function to Calculate Velocity between ECEF coordinates //
// ------------------------------------------------------- //
void calculateDiscreteVelocities(
    const std::vector<EcefData>& ecef_data,
    std::vector<HighPrecisionFloat>& velocity_time_vector,
    std::vector<HighPrecisionFloat>& velocity_x_vector,
    std::vector<HighPrecisionFloat>& velocity_y_vector,
    std::vector<HighPrecisionFloat>& velocity_z_vector
) {
    // Set the first coordinates' velocity to 0 as the instructions specify
    velocity_time_vector.push_back(ecef_data[0].time);
    velocity_x_vector.push_back(HighPrecisionFloat("0.0"));
    velocity_y_vector.push_back(HighPrecisionFloat("0.0"));
    velocity_z_vector.push_back(HighPrecisionFloat("0.0"));

    // Calculate discrete velocities starting from the second coordinates
    for (size_t i = 1; i < ecef_data.size(); ++i) {
        // Calculate the time difference (delta T)
        HighPrecisionFloat delta_T = ecef_data[i].time - ecef_data[i - 1].time;

        // Calculate the position difference (delta P) in ECEF x, y, z components
        HighPrecisionFloat delta_x = ecef_data[i].x - ecef_data[i - 1].x;
        HighPrecisionFloat delta_y = ecef_data[i].y - ecef_data[i - 1].y;
        HighPrecisionFloat delta_z = ecef_data[i].z - ecef_data[i - 1].z;

        // Calculate velocity (delta P / delta T)
        HighPrecisionFloat velocity_x = delta_x / delta_T;
        HighPrecisionFloat velocity_y = delta_y / delta_T;
        HighPrecisionFloat velocity_z = delta_z / delta_T;

        // Store the velocity and corresponding time
        velocity_time_vector.push_back(ecef_data[i].time);
        velocity_x_vector.push_back(velocity_x);
        velocity_y_vector.push_back(velocity_y);
        velocity_z_vector.push_back(velocity_z);
    }
}

// ------------------------------------------------------------ //
// Function to Get the Interpolated Velocity at a specific time //
// ------------------------------------------------------------ //
void getInterpolatedVelocity(
    HighPrecisionFloat time_to_find,
    const VelocityInterpolator& x_velocity_interpolator,
    const VelocityInterpolator& y_velocity_interpolator,
    const VelocityInterpolator& z_velocity_interpolator
) {
    HighPrecisionFloat velocity_x = x_velocity_interpolator(time_to_find);
    HighPrecisionFloat velocity_y = y_velocity_interpolator(time_to_find);
    HighPrecisionFloat velocity_z = z_velocity_interpolator(time_to_find);

    std::cout << std::format("ECEF Velocity Vector (Vx, Vy, Vz) at time {}:\n", time_to_find.str(30));
    std::cout << std::format("({}, {}, {}) m/s\n", velocity_x.str(30), velocity_y.str(30), velocity_z.str(30));
}


// ---------------------------------------------------------------- //
//                                                                  //
//   Main application to interpolate velocities at specific times   //
//                                                                  //
// ---------------------------------------------------------------- //
int main()
{
    // Get LLA data from the CSV file
    std::vector<LlaData> llaData = getLlaData(kFILEPATH);

    // Convert LLA coordinates to ECEF coordinates
    std::vector<EcefData> ecef_data = convertLlaToEcef(llaData);

    // Calculate velocity between ECEF coordinates
    std::vector<HighPrecisionFloat> velocity_time_vector;
    std::vector<HighPrecisionFloat> velocity_x_vector;
    std::vector<HighPrecisionFloat> velocity_y_vector;
    std::vector<HighPrecisionFloat> velocity_z_vector;

    calculateDiscreteVelocities(
        ecef_data,
        velocity_time_vector,
        velocity_x_vector,
        velocity_y_vector,
        velocity_z_vector
    );

    // Create three separate interpolators
    VelocityInterpolator x_velocity_interpolator, y_velocity_interpolator, z_velocity_interpolator;

    // Set the data for each interpolator (velocity with respect to time)
    x_velocity_interpolator.setData(velocity_time_vector, velocity_x_vector);
    y_velocity_interpolator.setData(velocity_time_vector, velocity_y_vector);
    z_velocity_interpolator.setData(velocity_time_vector, velocity_z_vector);

    // Get the Interpolated Velocity at a specific time
    HighPrecisionFloat target_time_1("1532334000.0");
    HighPrecisionFloat target_time_2("1532335268.0");

    getInterpolatedVelocity(
        target_time_1,
        x_velocity_interpolator,
        y_velocity_interpolator,
        z_velocity_interpolator
    );
    std::cout << "\n";

    getInterpolatedVelocity(
        target_time_2,
        x_velocity_interpolator,
        y_velocity_interpolator,
        z_velocity_interpolator
    );

    return 0;
}

