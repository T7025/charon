//
// Created by thomas on 4/27/18.
//

#include <gtest/gtest.h>
#include <impl1/BarnesHutUniverse.h>
#include <spdlog/sinks/null_sink.h>
#include <universeShapes/BinaryUniverseShape.h>
#include <impl1/BruteForceUniverse.h>
#include <universeShapes/RandomCubeUniverseShape.h>

TEST(BarnesHutForceUniverse, BinaryUniverseShape) {
    auto settings = std::make_shared<Settings>();
    settings->fileSettings->enableFileOutput = false;
    settings->universeSettings->timeStep = 0.001;
    settings->universeSettings->gravitationalConstant = 1.0;
    settings->universeSettings->useCollisions = false;

    auto console = std::make_shared<spdlog::logger>("console", std::make_shared<spdlog::sinks::null_sink_mt>());
    auto universe = std::make_shared<BarnesHutUniverse>(settings, console);
    universe->initBodies(std::make_shared<BinaryUniverseShape>());
    universe->calculateFirstStep();
    fp maxDeviation = 0;
    for (int i = 0; i < 10000; ++i) {
        universe->calculateNextStep();
        auto positions = universe->getPositions();
        auto dist1 = positions[0].norm();
        auto dist2 = positions[1].norm();
        maxDeviation = std::max(maxDeviation, std::abs(dist1 - 1.0));
        maxDeviation = std::max(maxDeviation, std::abs(dist2 - 1.0));
        //EXPECT_NEAR(dist1, 1.0, 0.00001);
        //EXPECT_NEAR(dist2, 1.0, 0.00001);
    }
    EXPECT_NEAR(maxDeviation, 0.0, 0.00001);
    std::cout << "Maximum error: " << maxDeviation << std::endl;
}

TEST(BarnesHutForceUniverse, CompareToBruteForce) {
    auto settings = std::make_shared<Settings>();
    settings->fileSettings->enableFileOutput = false;
    settings->universeSettings->timeStep = 0.001;
    settings->universeSettings->gravitationalConstant = 1.0;
    settings->universeSettings->useCollisions = false;
    settings->universeSettings->number_bodies = 256;
    auto console = std::make_shared<spdlog::logger>("console", std::make_shared<spdlog::sinks::null_sink_mt>());

    auto bruteForceUniverse = std::make_shared<BruteForceUniverse>(settings, console);
    auto barnesHutUniverse = std::make_shared<BarnesHutUniverse>(settings, console);

    bruteForceUniverse->initBodies(std::make_shared<RandomCubeUniverseShape>());
    barnesHutUniverse->initBodies(std::make_shared<RandomCubeUniverseShape>());
    fp maxDeviation = 0;
    for (int i = 0; i < 100; ++i) {
        bruteForceUniverse->calculateNextStep();
        barnesHutUniverse->calculateNextStep();

        for (int bodyIndex = 0; bodyIndex < settings->universeSettings->number_bodies; ++bodyIndex) {
            auto pos1 = bruteForceUniverse->getPositions()[bodyIndex];
            auto pos2 = barnesHutUniverse->getPositions()[bodyIndex];
            maxDeviation = std::max(maxDeviation, pos1.distance(pos2));
        }
    }
    EXPECT_NEAR(maxDeviation, 0.0, 1E-5);
    std::cout << "Maximum error: " << maxDeviation << std::endl;
}