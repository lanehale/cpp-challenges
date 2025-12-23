# Geodetic Coordinate Conversion (LLA → ECEF)

Personal C++ challenge implementing transformation from **Latitude/Longitude/Altitude (LLA)** to **Earth-Centered, Earth-Fixed (ECEF)** coordinates using the WGS84 ellipsoid model.

## Overview
- Converts geodetic coordinates (degrees, meters) to Cartesian ECEF (X, Y, Z in meters)
- Includes velocity interpolation at arbitrary times
- Demonstrates precision floating-point math, trigonometric calculations, and modern C++ practices

Built as a self-directed coding challenge to deepen systems-level C++ skills.
'''
************************************************************
     INSTRUCTIONS FOR BUILDING AND RUNNING (LINUX)
************************************************************

This project uses CMake to manage the build process and vcpkg to manage
dependencies (Boost and libInterpolate). All of the coordinate conversions
and linear interpolations use numbers with high precision decimal places
that exceed the incoming LLA numeric data precision of 24 decimal places.


============================================================
 STEP 1: INSTALL SYSTEM PREREQUISITES AND VCPKG
============================================================

First, ensure the necessary Linux system build tools are installed.
From any directory in your Linux shell, run:
--------------------------------------------
    sudo apt update
    sudo apt install build-essential pkg-config autoconf autoconf-archive automake libtool


Next, clone the vcpkg repository and install the libraries:
-----------------------------------------------------------
    git clone https://github.com/microsoft/vcpkg.git
    cd ~/vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg install boost-multiprecision libinterpolate

[NOTE]: The last install command downloads and compiles the libraries from source.
        This process typically takes 1-2 minutes to complete the first time.


============================================================
 STEP 2: CONFIGURE THE PROJECT BUILD WITH CMAKE
============================================================

Navigate to the project directory, create a build directory, 
copy the necessary data file, and run CMake:
--------------------------------------------
    cd ~/CoordinateConversion
    mkdir build && cd build
    cp ../sample_lla_data.csv .
    cmake -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake ..


============================================================
 STEP 3: BUILD AND RUN THE EXECUTABLE
============================================================

From the /build directory:
--------------------------
    make
    ./CoordinateConversion


============================================================
 EXPECTED OUTPUT:
============================================================

~/CoordinateConversion/build$ ./CoordinateConversion
ECEF Velocity Vector (Vx, Vy, Vz) at time 1532334000:
(-995.915268655830842075665801648, -2514.43889405035981285082106335, 55.9212201842264911203061142062) m/s

ECEF Velocity Vector (Vx, Vy, Vz) at time 1532335268:
(-3471.02128301681392006119377892, 1760.25788761925766725589689167, -4867.47606248494785073659059015) m/s
~/CoordinateConversion/build$ 
'''
