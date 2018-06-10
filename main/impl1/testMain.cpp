//
// Created by thomas on 5/12/18.
//

#include <settings/Settings.h>
#include <spdlog/sinks/null_sink.h>
//#include "BruteForceOffloadUniverse.h"
#include "BruteForceUniverse.h"
#include "BruteForceMultiThreadUniverse.h"
//#include "BruteForceOffloadUniverse2.h"
#include "BruteForceOffloadUniverse3.h"
#include "BarnesHutUniverse2.h"
#include <universeShapes/RandomCubeUniverseShape.h>
#include <iostream>
#include <base/Timer.h>
#include <bitset>

/*void test() {
    auto settings = std::make_shared<Settings>();
    settings->fileSettings->enableFileOutput = false;
    settings->universeSettings->timeStep = 0.001;
    settings->universeSettings->gravitationalConstant = 1.0;
    settings->universeSettings->useCollisions = false;
    settings->universeSettings->number_bodies = 2u<<21u;
    auto console = std::make_shared<spdlog::logger>("console", std::make_shared<spdlog::sinks::null_sink_mt>());

    auto universe = std::make_shared<BruteForceOffloadUniverse>(settings, console);

    Timer timer;
    universe->initBodies(std::make_shared<RandomCubeUniverseShape>());
    std::cout << "Setup time: " <<timer.lap()<<" sec (nBodies: "<< universe->bodyCount() << ")\n";
    int nRuns = 100/2;
    *//*
    for (int i = 0; i < nRuns; ++i) {
        //std::cout << "Run " << i << ": " << timer.lap() << " sec\n";
    }*//*
    universe->calcNextPosition();
    const auto runTime = timer.lap();
    std::cout << "Total run time: " << runTime << " sec, avg. time: " << runTime/nRuns << " sec\n";
    std::cout << "Total time: "<< timer.finish() << " sec\n";
}

void test2() {
    auto settings = std::make_shared<Settings>();
    settings->fileSettings->enableFileOutput = false;
    settings->universeSettings->timeStep = 0.001;
    settings->universeSettings->gravitationalConstant = 1.0;
    settings->universeSettings->useCollisions = false;
    settings->universeSettings->number_bodies = 2u<<21u;
    auto console = std::make_shared<spdlog::logger>("console", std::make_shared<spdlog::sinks::null_sink_mt>());

    auto universe = std::make_shared<BruteForceOffloadUniverse2Interface>(settings, console);

}*/

void test3() {
    auto settings = std::make_shared<Settings>();
    settings->fileSettings->enableFileOutput = false;
    settings->universeSettings->timeStep = 0.001;
    settings->universeSettings->gravitationalConstant = 1.0;
    settings->universeSettings->useCollisions = false;
//    settings->universeSettings->number_bodies = 2u<<21u;
    settings->universeSettings->number_bodies = 1u<<16u;
    auto console = std::make_shared<spdlog::logger>("console", std::make_shared<spdlog::sinks::null_sink_mt>());

    auto universe = std::make_shared<BruteForceOffloadUniverse3>(settings, console);
//    auto universe = std::make_shared<BruteForceMultiThreadUniverse>(settings, console);

    Timer timer;
    universe->initBodies(std::make_shared<RandomCubeUniverseShape>());
    std::cout << "Setup time: " <<timer.lap()<<" sec (nBodies: "<< universe->bodyCount() << ")\n";
    int nRuns = 10;
    for (int i = 0; i < nRuns; ++i) {
//        universe->calcNextPosition();
        universe->calculateNextStep();
        //std::cout << "Run " << i << ": " << timer.lap() << " sec\n";
    }
    const auto runTime = timer.lap();
    std::cout << "Total run time: " << runTime << " sec, avg. time: " << runTime/nRuns << " sec\n";
    std::cout << "Total time: "<< timer.finish() << " sec\n";
}

void test4() {
    auto settings = std::make_shared<Settings>();
    settings->fileSettings->enableFileOutput = false;
    settings->universeSettings->timeStep = 0.001;
    settings->universeSettings->gravitationalConstant = 1.0;
    settings->universeSettings->useCollisions = false;
    settings->universeSettings->number_bodies = 1u<<22u;
    auto console = std::make_shared<spdlog::logger>("console", std::make_shared<spdlog::sinks::null_sink_mt>());
    BarnesHutUniverse2 universe{settings, console};
    universe.initBodies(std::make_shared<RandomCubeUniverseShape>());

//    for (const auto &pos : universe.getPositions()) {
//        std::cout << pos << "\n";
//        auto sfcIndex = universe.getSFC(Vector3Offload{pos}, 1, 20);
//        std::cout << "  " << std::bitset<sizeof(unsigned)*8>{sfcIndex} << "\n";
//    }
    universe.buildTree();
}

int main() {
    test4();
    return 0;
}