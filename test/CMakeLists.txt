
include(CTest)

set(GTEST_SRC
        BruteForceUniverseTest.cpp
        main.cpp BarnesHutUniverseTest.cpp)

add_executable(runUnitTests ${GTEST_SRC})
target_link_libraries(runUnitTests libcharon gtest gmock_main)

add_test(NAME runUnitTests COMMAND runUnitTests)
