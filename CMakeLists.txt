cmake_minimum_required(VERSION 3.9)

include(CMakeConfig.cmake)
include(CMakeLocal.cmake OPTIONAL)

#============================================================================
# Configuration for CMake, for build & install.
#============================================================================
project(charon)

#============================================================================
# Configuration for CPP.
#============================================================================
include(CMakeCPP.cmake)

#============================================================================
# Configuration for DOC: DOXYGEN
#============================================================================
if (CHARON_INCLUDE_DOC)
    find_package(Doxygen)
endif ()

#============================================================================
# Add subdirs
#============================================================================
add_subdirectory(external)
add_subdirectory(main)
add_subdirectory(main/simple1)  # charon01
add_subdirectory(test)
add_subdirectory(benchmark)
if (STRIDE_INCLUDE_DOC)
    add_subdirectory(doc)
endif ()

#============================================================================
# Overview report:
#============================================================================
include(CMakeReport.cmake)

