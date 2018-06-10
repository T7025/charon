#----------------------------------------------------------------------------
# Check CMAKE_BUILD_TYPE
#----------------------------------------------------------------------------
if (NOT (CMAKE_BUILD_TYPE MATCHES "Release" OR CMAKE_BUILD_TYPE MATCHES "Debug"))
    message(FATAL_ERROR "========> CMAKE_BUILD_TYPE HAS TO MATCH EITHER Release OR Debug.")
endif ()

#----------------------------------------------------------------------------
# Compile flags
#----------------------------------------------------------------------------
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wno-unknown-pragmas ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Og -Ofast -march=native") # -fverbose-asm") # -march=native")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-array-bounds")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-math-errno")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funroll-loops")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorize -fopt-info-vec-all=info_vec.txt")  # -fopt-info-vec-optimized/missed/note/all
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -foffload=-lm")  # Enable math functions that cannot be directly generated as PTX instructions
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og")
else ()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
endif ()

#----------------------------------------------------------------------------
# Platform dependent compile flags
#----------------------------------------------------------------------------
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_HOST_APPLE)
    add_definitions(-D__APPLE__)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-private-field -stdlib=libc++")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT CMAKE_HOST_APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wno-unused-private-field -Wno-unused-command-line-argument")
    add_definitions(-D__extern_always_inline=inline)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wno-maybe-uninitialized")
endif ()
#
include_directories(${CMAKE_HOME_DIRECTORY}/main)

#----------------------------------------------------------------------------
# Standard math lib
#----------------------------------------------------------------------------
set(LIBS ${LIBS} m)

#----------------------------------------------------------------------------
# Spdlog Library (logging)
#----------------------------------------------------------------------------
#include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/resources/spdlog/include)

#----------------------------------------------------------------------------
# Tina's Random Number Generator (TRNG) library and paths
#----------------------------------------------------------------------------
#include_directories(SYSTEM ${CMAKE_HOME_DIRECTORY}/main/resources/lib/trng-4.15/include)
#set(LIBS ${LIBS} trng)

#----------------------------------------------------------------------------
# Boost
#----------------------------------------------------------------------------
set(BOOST_ROOT ${STRIDE_BOOST_ROOT})
set(Boost_NO_SYSTEM_PATHS ${STRIDE_BOOST_NO_SYSTEM_PATHS})
message(status ${Boost_ROOT})
find_package(Boost COMPONENTS filesystem thread date_time system REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

#----------------------------------------------------------------------------
# OpenMP
#----------------------------------------------------------------------------
if (NOT STRIDE_FORCE_NO_OPENMP)
    if (NOT DEFINED HAVE_CHECKED_OpenMP)
        set(HAVE_CHECKED_OpenMP TRUE CACHE BOOL "Have checked for OpenMP?" FORCE)
        find_package(OpenMP)
        if (OPENMP_FOUND)
            set(HAVE_FOUND_OpenMP TRUE CACHE BOOL "Have found OpenMP?" FORCE)
        else ()
            set(HAVE_FOUND_OpenMP FALSE CACHE BOOL "Have found OpenMP?" FORCE)
        endif ()
    endif ()
    if (HAVE_FOUND_OpenMP)
        set(OPENMP_FOUND TRUE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    else ()
        # This is done to eliminate blank output of undefined CMake variables.
        set(OPENMP_FOUND FALSE)
    endif ()
else ()
    # This is done to eliminate blank output of undefined CMake variables.
    set(OPENMP_FOUND FALSE)
    set(HAVE_CHECKED_OpenMP FALSE)
    set(HAVE_FOUND_OpenMP FALSE)
endif ()
# If not found, use the dummy omp.
if (NOT OPENMP_FOUND)
    include_directories(${CMAKE_HOME_DIRECTORY}/main/resources/lib/domp/include)
endif ()

#----------------------------------------------------------------------------
# HDF5 Library
# Try to find the C variant of libhdf5, if found, USE_HDF5 is defined
# and passed to the compilers to allow compilation of selective features
# through preprocessor commands like #ifdef USE_HDF5 and friends.
# Additional defs are required on Ubuntu where lib are installed 
# with hdf5 v1.6 as default behavior.
#----------------------------------------------------------------------------
#if (STRIDE_FORCE_NO_HDF5)
#    message(STATUS "---> Skipping HDF5, STRIDE_FORCE_NO_HDF5 set.")
#else ()
#    find_package(HDF5 COMPONENTS C HL)
#    if (HDF5_FOUND)
#        include_directories(SYSTEM ${HDF5_INCLUDE_DIRS})
#        set(LIBS ${LIBS} ${HDF5_LIBRARIES})
#        add_definitions(-DUSE_HDF5 -DH5_NO_DEPRECATED_SYMBOLS)
#        add_definitions(-DH5Dcreate_vers=2 -DH5Dopen_vers=2)
#        add_definitions(-DH5Acreate_vers=2 -DH5Gcreate_vers=2)
#        add_definitions(-DH5Gopen_vers=2)
#    else ()
#        # This is done to eliminate blank output of undefined CMake variables.
#        set(HDF5_FOUND FALSE)
#    endif ()
#endif ()

#############################################################################
