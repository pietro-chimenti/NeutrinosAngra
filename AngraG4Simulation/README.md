# AngraG4Simulation

Angra Simulation using Geant4 for neutrino detector simulation.

## Authors
- P. Chimenti
- R. Lima
- G. Valdiviesso

## Building the Project

### Prerequisites
- CMake 3.16 or higher
- Geant4 10.6 or higher
- C++ compiler with C++17 support

### Build Instructions

```bash
# Create a build directory
mkdir build
cd build

# Configure the project
cmake ..

# Build the project
cmake --build .

# Run the simulation
./bin/AngraG4Simulation
```

## Configuration Options

- `WITH_GEANT4_UIVIS`: Build with Geant4 UI and Vis drivers (default: ON)
- `CMAKE_BUILD_TYPE`: Set the build type (Debug, Release, RelWithDebInfo, MinSizeRel)
- `CMAKE_INSTALL_PREFIX`: Set the installation directory

Example:
```bash
cmake -DWITH_GEANT4_UIVIS=OFF -DCMAKE_BUILD_TYPE=Release ..
```

## Installation

```bash
cmake --build . --target install
```

## Uninstallation

```bash
cmake --build . --target uninstall
```
