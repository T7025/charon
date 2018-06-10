set(LIB_SRC
        simulator/Simulator.cpp
        base/AbstractUniverse.cpp
        base/AbstractUniverse.h
        base/Util.h
        base/Vector3.cpp
        impl1/BruteForceUniverse.cpp
        impl1/BruteForceUniverse.h
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
        impl1/BarnesHutUniverse.cpp
        impl1/BarnesHutUniverse.h
        impl1/OctTreeNode.cpp
        impl1/OctTreeNode.h
        base/Timer.h
        impl1/BruteForceMultiThreadUniverse.cpp
        impl1/BruteForceMultiThreadUniverse.h
#        impl1/BruteForceOffloadUniverse.cpp
#        impl1/BruteForceOffloadUniverse.h
        base/Vector3Offload.cpp
        base/Vector3Offload.h
        #impl1/BruteForceOffloadUniverse2.cpp impl1/BruteForceOffloadUniverse2.h
        impl1/BruteForceOffloadUniverse3.cpp impl1/BruteForceOffloadUniverse3.h
        impl1/BarnesHutUniverse2.cpp impl1/BarnesHutUniverse2.h
        impl1/SFCOctTree/SFCOctTree.cpp impl1/SFCOctTree/SFCOctTree.h
        impl1/SFCOctTree/Node.cpp impl1/SFCOctTree/Node.h
        impl1/SFCOctTree/SFCIndex.cpp impl1/SFCOctTree/SFCIndex.h
        impl1/SFCOctTree/constants.h
        impl1/BarnesHutUniverse3.cpp impl1/BarnesHutUniverse3.h)

add_library(libcharon ${LIB_SRC})
#target_link_libraries(libcharon ${LIBS})
target_link_libraries(libcharon spdlog)

add_executable(charon simulator/main.cpp)
#target_include_directories(charon )
target_link_libraries(charon ${LIBS} libcharon)

add_executable(octTreeMain impl1/octTreeMain.cpp)

add_executable(SFCTree impl1/SFCTree.cpp)

add_executable(testMain impl1/testMain.cpp)
target_link_libraries(testMain libcharon)

add_executable(sfcOctTreeTest impl1/SFCOctTree/sfcOctTreeTest.cpp)
target_link_libraries(sfcOctTreeTest libcharon)
