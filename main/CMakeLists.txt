set(
        LIB_SRC
        simulator/Simulator.cpp
        base/AbstractUniverse.cpp
        base/AbstractUniverse.h
        base/Util.h
        base/Vector3.cpp
        bruteForceImpl/BruteForceUniverse.cpp
        bruteForceImpl/BruteForceUniverse.h
        universeShapes/UniverseShape.cpp
        universeShapes/UniverseShape.h
        universeShapes/BinaryUniverseShape.h
        settings/Settings.cpp
        settings/Settings.h
        settings/FileSettings.cpp
        settings/FileSettings.h
        settings/SimulatorSettings.cpp
        settings/SimulatorSettings.h
        settings/UniverseSettings.cpp
        settings/UniverseSettings.h
        universeShapes/RandomCubeUniverseShape.h
        universeShapes/UniverseShapes.cpp
        universeShapes/UniverseShapes.h
        barnesHutTreeImpl/BarnesHutTreeUniverse.cpp
        barnesHutTreeImpl/BarnesHutTreeUniverse.h
        barnesHutTreeImpl/OctTreeNode.cpp
        barnesHutTreeImpl/OctTreeNode.h
        base/Timer.h
        bruteForceImpl/BruteForceMultiThreadUniverse.cpp
        bruteForceImpl/BruteForceMultiThreadUniverse.h
        #        impl1/BruteForceOffloadUniverse.cpp
        #        impl1/BruteForceOffloadUniverse.h
        base/Vector3.cpp
        base/Vector3.h
        #impl1/BruteForceOffloadUniverse2.cpp impl1/BruteForceOffloadUniverse2.h
        bruteForceImpl/BruteForceOffloadUniverse3.cpp bruteForceImpl/BruteForceOffloadUniverse3.h
        impl1/BarnesHutUniverse2.cpp impl1/BarnesHutUniverse2.h
        impl1/SFCOctTree/SFCOctTree.cpp impl1/SFCOctTree/SFCOctTree.h
        impl1/SFCOctTree/Node.cpp impl1/SFCOctTree/Node.h
        impl1/SFCOctTree/SFCIndex.cpp impl1/SFCOctTree/SFCIndex.h
        impl1/SFCOctTree/constants.h
        impl1/BarnesHutUniverse3.cpp impl1/BarnesHutUniverse3.h
        bruteForceImpl/BruteForceUniverseBase.cpp bruteForceImpl/BruteForceUniverseBase.h
        barnesHutSFCImpl/BarnesHutUniverseBase.cpp barnesHutSFCImpl/BarnesHutUniverseBase.h
        barnesHutSFCImpl/BarnesHutUniverse.cpp barnesHutSFCImpl/BarnesHutUniverse.h
        barnesHutSFCImpl/BarnesHutMultiThreadUniverse.cpp barnesHutSFCImpl/BarnesHutMultiThreadUniverse.h
        base/intel_stable_sort/parallel_stable_sort.h
        base/intel_stable_sort/pss_common.h
        barnesHutSFCImpl/BarnesHutOffloadUniverse.cpp barnesHutSFCImpl/BarnesHutOffloadUniverse.h
)

add_library(libcharon ${LIB_SRC})
#target_link_libraries(libcharon ${LIBS})
target_link_libraries(libcharon spdlog)

add_executable(charon simulator/main.cpp)
#target_include_directories(charon )
target_link_libraries(charon ${LIBS} libcharon)

add_executable(charon_benchmark simulator/benchmark.cpp)
target_link_libraries(charon_benchmark ${LIBS} libcharon)

add_executable(octTreeMain impl1/octTreeMain.cpp)

add_executable(SFCTree impl1/SFCTree.cpp)

add_executable(testMain impl1/testMain.cpp)
target_link_libraries(testMain libcharon)

add_executable(sfcOctTreeTest impl1/SFCOctTree/sfcOctTreeTest.cpp)
target_link_libraries(sfcOctTreeTest libcharon)
