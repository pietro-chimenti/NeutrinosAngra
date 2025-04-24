#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(AngraG4Simulation
        VERSION 1.0.0
        DESCRIPTION "Angra Simulation using Geant4"
        LANGUAGES CXX)

# Add cmake modules path
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif()

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# Set C++ standard based on Geant4 version
set(CMAKE_CXX_STANDARD ${Geant4_CXXSTD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add compile options based on compiler
#
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_compile_options(-Wall -Wextra -pedantic)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -pedantic)
elseif(MSVC)
  add_compile_options(/W4)
endif()

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(AngraG4Simulation AngraG4Simulation.cc ${sources} ${headers})
target_include_directories(AngraG4Simulation PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(AngraG4Simulation PRIVATE ${Geant4_LIBRARIES})

# Add compile definitions for different build types
target_compile_definitions(AngraG4Simulation PRIVATE
  $<$<CONFIG:Debug>:DEBUG>
  $<$<CONFIG:Release>:NDEBUG>
)

# Set output directory for the executable
set_target_properties(AngraG4Simulation PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory's bin folder, i.e. the directory in which we
# build AngraG4Simulation. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(ANGRAG4SIMULATION_SCRIPTS
  constants.dat
  run1.mac
  vis.mac
  vis_qt.mac
  )

# Create bin directory if it doesn't exist
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

foreach(_script ${ANGRAG4SIMULATION_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/bin/${_script}
    COPYONLY
    )
  # Also copy to the main build directory for backward compatibility
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Installation
#
include(GNUInstallDirs)

# Install the executable
install(TARGETS AngraG4Simulation
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Install the scripts
install(FILES ${ANGRAG4SIMULATION_SCRIPTS}
        DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})

# Install README and other documentation
if(EXISTS "${PROJECT_SOURCE_DIR}/README.md")
  install(FILES "${PROJECT_SOURCE_DIR}/README.md"
          DESTINATION ${CMAKE_INSTALL_DOCDIR})
endif()

# Create a basic uninstall target
# (this is not provided by CMake by default)
configure_file(
  "${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  COMMAND ${CMAKE_COMMAND} -P ${PROJECT_BINARY_DIR}/cmake_uninstall.cmake)

#----------------------------------------------------------------------------
# Print build configuration summary
#
message(STATUS "")
message(STATUS "=== Build Configuration ===")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "Geant4 Version: ${Geant4_VERSION}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
