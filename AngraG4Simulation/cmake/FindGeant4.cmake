# - Find Geant4 library
# This module sets up Geant4 information
# It defines:
# Geant4_FOUND               If the Geant4 is found
# Geant4_INCLUDE_DIRS        PATH to the include directories
# Geant4_LIBRARIES           Libraries to link against
# Geant4_CXXSTD              C++ Standard used by Geant4

# Find Geant4 using the standard Geant4Config.cmake script
find_package(Geant4 QUIET CONFIG)

if(Geant4_FOUND)
  # Determine the C++ standard used by Geant4 by checking the compiler flags
  if(NOT DEFINED Geant4_CXXSTD)
    # Try to find the C++ standard from geant4-config
    find_program(GEANT4_CONFIG geant4-config HINTS
                 ${Geant4_DIR}/../../bin
                 $ENV{GEANT4_DIR}/bin
                 PATH_SUFFIXES bin)

    if(GEANT4_CONFIG)
      execute_process(
        COMMAND ${GEANT4_CONFIG} --cflags
        OUTPUT_VARIABLE GEANT4_CFLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )

      # Extract C++ standard from flags
      if(GEANT4_CFLAGS MATCHES "-std=c\\+\\+(([0-9]+)|([0-9]+x))")
        string(REGEX REPLACE ".*-std=c\\+\\+([0-9]+).*" "\\1" Geant4_CXXSTD "${GEANT4_CFLAGS}")
        # Handle c++0x, c++1y, etc. format
        if(Geant4_CXXSTD MATCHES "0x")
          set(Geant4_CXXSTD "11")
        elseif(Geant4_CXXSTD MATCHES "1y")
          set(Geant4_CXXSTD "14")
        elseif(Geant4_CXXSTD MATCHES "1z")
          set(Geant4_CXXSTD "17")
        elseif(Geant4_CXXSTD MATCHES "2a")
          set(Geant4_CXXSTD "20")
        endif()
      else()
        # Fallback to version-based detection if no -std flag found
        if(${Geant4_VERSION} VERSION_GREATER_EQUAL 11.0.0)
          set(Geant4_CXXSTD 17)
        elseif(${Geant4_VERSION} VERSION_GREATER_EQUAL 10.6.0)
          set(Geant4_CXXSTD 14)
        else()
          set(Geant4_CXXSTD 11)
        endif()
        message(STATUS "Could not detect C++ standard from geant4-config, using version-based detection")
      endif()
    else()
      # Fallback to version-based detection if geant4-config not found
      if(${Geant4_VERSION} VERSION_GREATER_EQUAL 11.0.0)
        set(Geant4_CXXSTD 17)
      elseif(${Geant4_VERSION} VERSION_GREATER_EQUAL 10.6.0)
        set(Geant4_CXXSTD 14)
      else()
        set(Geant4_CXXSTD 11)
      endif()
      message(STATUS "geant4-config not found, using version-based detection for C++ standard")
    endif()
  endif()

  # Print Geant4 information
  if(NOT Geant4_FIND_QUIETLY)
    message(STATUS "Found Geant4: ${Geant4_DIR} (version ${Geant4_VERSION})")
    message(STATUS "Geant4 C++ Standard: C++${Geant4_CXXSTD}")
    if(DEFINED GEANT4_CFLAGS)
      message(STATUS "Geant4 CFLAGS: ${GEANT4_CFLAGS}")
    endif()
  endif()
endif()
