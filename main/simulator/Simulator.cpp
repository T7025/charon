//
// Created by thomas on 3/13/18.
//

#include "Simulator.h"
#include <boost/property_tree/detail/ptree_implementation.hpp>
#include <boost/property_tree/ptree.hpp>
#include <universeShapes/BinaryUniverseShape.h>
#include <bruteForceImpl/BruteForceUniverse.h>
#include <universeShapes/RandomCubeUniverseShape.h>
#include <barnesHutSFCImpl/BarnesHutUniverse.h>
#include <barnesHutTreeImpl/BarnesHutTreeUniverse.h>
#include <bruteForceImpl/BruteForceMultiThreadUniverse.h>
#include <bruteForceImpl/BruteForceOffloadUniverse3.h>
#include <barnesHutSFCImpl/BarnesHutMultiThreadUniverse.h>
#include <barnesHutSFCImpl/BarnesHutOffloadUniverse.h>
//#include <impl1/BarnesHutUniverse2.h>
//#include <impl1/BarnesHutUniverse3.h>

Simulator::Simulator(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console) :
        nrOfSteps{settings->simulatorSettings->nrIterations},
        snapshotDelta{settings->simulatorSettings->snapshotDelta},
        settings{std::move(settings)},
        console{std::move(console)} {}

void Simulator::setup2() {
    const auto universeType = settings->simulatorSettings->universe_type;
    if (universeType == "brute_force") {
        universe = std::make_shared<BruteForceUniverse>(settings, console);
    } else if (universeType == "brute_force_multi_thread") {
        universe = std::make_shared<BruteForceMultiThreadUniverse>(settings, console);
    } else if (universeType == "brute_force_offload") {
        universe = std::make_shared<BruteForceOffloadUniverse3>(settings, console);
    } else if (universeType == "barnes_hut_tree") {
        universe = std::make_shared<BarnesHutTreeUniverse>(settings, console);
    } else if (universeType == "barnes_hut") {
        universe = std::make_shared<BarnesHutUniverse>(settings, console);
    } else if (universeType == "barnes_hut_multi_thread") {
        universe = std::make_shared<BarnesHutMultiThreadUniverse>(settings, console);
    } else if (universeType == "barnes_hut_offload") {
        universe = std::make_shared<BarnesHutOffloadUniverse>(settings, console);
//    } else if (universeType == "barnes_hut3") {
//        universe = std::make_shared<BarnesHutUniverse3>(settings, console);
    } else {
        throw std::runtime_error{"simulatorSettings: not a valid universe type"};
    }
    const auto universeShapeName = settings->simulatorSettings->universe_shape;
    std::shared_ptr<UniverseShape> universeShape;
    if (universeShapeName == "binary") {
        universeShape = std::make_shared<BinaryUniverseShape>();
    } else if (universeShapeName == "random_cube") {
        universeShape = std::make_shared<RandomCubeUniverseShape>();
    } else {
        throw std::runtime_error{"simulatorSettings: not a valid universe shape"};
    }
    universe->initBodies(universeShape);
}

void Simulator::run() {
    if (nrOfSteps > 0) {
        universe->calculateFirstStep();
    }
    snapShot();
    for (unsigned i = 1; i < nrOfSteps; ++i) {
        const auto startStepTime = std::chrono::high_resolution_clock::now();
        universe->calculateNextStep();
        std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - startStepTime;
        console->info("Duration step {}: {} seconds", i, duration.count());
        if (i % snapshotDelta == 0 && settings->fileSettings->enableFileOutput) {
            snapShot();
        }
    }
    snapShot();
}

void Simulator::snapShot() const {
    universe->logInternalState();
}
