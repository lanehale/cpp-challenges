# Geodetic Coordinate Conversion (LLA → ECEF)

Personal C++ challenge implementing transformation from **Latitude/Longitude/Altitude (LLA)** to **Earth-Centered, Earth-Fixed (ECEF)** coordinates using the WGS84 ellipsoid model.

## Overview
- Converts geodetic coordinates (degrees, meters) to Cartesian ECEF (X, Y, Z in meters)
- Includes velocity interpolation at arbitrary times
- Demonstrates precision floating-point math, trigonometric calculations, and modern C++ practices

## Key Features
- Accurate handling of WGS84 ellipsoid parameters
- Edge-case support (poles, altitude variations)
- Clean, commented code with function-level documentation

Built as a self-directed coding challenge to deepen systems-level C++ skills.
