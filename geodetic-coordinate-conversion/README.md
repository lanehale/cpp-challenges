# Geodetic Coordinate Conversion (LLA → ECEF)

Personal C++ challenge implementing transformation from **Latitude/Longitude/Altitude (LLA)** to **Earth-Centered, Earth-Fixed (ECEF)** coordinates using the WGS84 ellipsoid model.

## Overview
- Converts geodetic coordinates (degrees, meters) to Cartesian ECEF (X, Y, Z in meters)
- Includes velocity interpolation at arbitrary times
- Demonstrates precision floating-point math, trigonometric calculations, and modern C++ practices

Built as a self-directed coding challenge to deepen systems-level C++ skills.

## Build & Run Instructions (Linux)

This project uses **CMake** and **vcpkg** for dependencies (Boost.Multiprecision and libInterpolate).

### Step 1: Install Prerequisites & vcpkg
```bash
sudo apt update
sudo apt install build-essential pkg-config autoconf autoconf-archive automake libtool

git clone https://github.com/microsoft/vcpkg.git
cd ~/vcpkg
./bootstrap-vcpkg.sh
./vcpkg install boost-multiprecision libinterpolate
```

### Step 2: Configure with CMake
```bash
cd path/to/CoordinateConversion
mkdir build && cd build
cp ../sample_lla_data.csv .
cmake -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake ..
```

### Step 3: Build & Run
```bash
make
./CoordinateConversion
```

### Expected Output
```bash
ECEF Velocity Vector (Vx, Vy, Vz) at time 1532334000:
(-995.915268655830842075665801648, -2514.43889405035981285082106335, 55.9212201842264911203061142062) m/s

ECEF Velocity Vector (Vx, Vy, Vz) at time 1532335268:
(-3471.02128301681392006119377892, 1760.25788761925766725589689167, -4867.47606248494785073659059015) m/s
```
