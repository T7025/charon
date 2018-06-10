
include(DownloadProject.cmake)

if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

# Google test
download_project(
        PROJ googletest
        GIT_REPOSITORY "https://github.com/google/googletest.git"
        GIT_TAG "ec44c6c1675c25b9827aacd08c02433cccde7780"  # latest release as of 18/02/2018
        ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

# Celero
download_project(
        PROJ celero
        GIT_REPOSITORY "https://github.com/DigitalInBlue/Celero.git"
        GIT_TAG "6b8a4bbc30c1771bd54c47b2f3ec229a74d9db1a"  # latest release as of 18/02/2018
        ${UPDATE_DISCONNECTED_IF_AVAILABLE}
        CMAKE_ARGS "-DCELERO_ENABLE_EXPERIMENTS=OFF;-DCMAKE_BUILD_TYPE=Release;-DCELERO_COMPILE_DYNAMIC_LIBRARIES=OFF"
)

add_subdirectory(${celero_SOURCE_DIR} ${celero_BINARY_DIR})

# Spdlog
download_project(
        PROJ spdlog
        GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
        GIT_TAG "ccd675a286f457068ee8c823f8207f13c2325b26"  # latest release as of 18/02/2018
        ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
