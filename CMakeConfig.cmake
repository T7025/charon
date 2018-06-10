
#============================================================================
# Configuration for the CMake tool itself.
#============================================================================
#set(CMAKE_ENABLE_COMPILE_COMMANDS      ON)
#set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS  TRUE)
#set(CMAKE_COLOR_MAKEFILE               ON)
#set(CMAKE_VERBOSE_MAKEFILE             OFF)
#enable_testing()

#============================================================================
# Override system defaults.
#============================================================================
#set(CMAKE_INSTALL_PREFIX  "${CMAKE_BINARY_DIR}/installed"
#        CACHE PATH "Install prefix path.")
#set(CMAKE_BUILD_TYPE          "Release"
#        CACHE STRING "Build type: None Debug Release RelWithDebInfo MinSizeRel.")
#set(CMAKE_PROGRAM_PATH  "/opt/local/bin;/usr/texbin;/usr/local/bin;/usr/bin"
#        CACHE PATH "Where to look with find_program." )

#============================================================================
# Using CCache if available.
#============================================================================
find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM AND NOT FORCE_NO_CCHACHE)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif ()

#============================================================================
# Charon specific variables:
#============================================================================
set(CHARON_INCLUDE_DOC FALSE
        CACHE BOOL "Exclude doc directory from build and install.")
set(CHARON_FORCE_NO_OPENMP FALSE
        CACHE BOOL "Do NOT use OpenMP even if available.")
set(CHARON_FORCE_NO_OPENACC FALSE
        CACHE BOOL "Do NOT use OpenACC even if available.")
set(CHARON_VERBOSE_TESTING TRUE
        CACHE BOOL "Run tests in verbose mode.")

#============================================================================
# Macro sets NAME to VALUE iff the NAME has not been defined yet:
#============================================================================
macro(set_if_null NAME VALUE)
    if (NOT DEFINED ${NAME} OR "${NAME}" STREQUAL "")
        set(${NAME} "${VALUE}")
    endif ()
endmacro(set_if_null)

#============================================================================
# Macro removes flag from CMAKE_CXX_FLAGS:
#============================================================================
macro(remove_cxx_flag flag)
    string(REPLACE "${flag}" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endmacro()

#############################################################################
