//
// Created by thomas on 5/8/18.
//

#include <celero/TestFixture.h>
#include <base/AbstractUniverse.h>
#include <bruteForceImpl/BruteForceUniverse.h>
#include <universeShapes/RandomCubeUniverseShape.h>
#include <spdlog/sinks/null_sink.h>
#include <celero/Celero.h>
#include <barnesHutTreeImpl/BarnesHutTreeUniverse.h>
#include <bruteForceImpl/BruteForceMultiThreadUniverse.h>
#include <bruteForceImpl/BruteForceOffloadUniverse3.h>
#include "defaultSettings.h"


template<typename UniverseType, typename UniverseShape, int MinIterations, typename ComplexityScaler>
class UniverseFixture : public celero::TestFixture {
public:


    void setUp(int64_t experimentValue) override {
        auto settings = getDefaultSettings();
        settings->universeSettings->number_bodies = static_cast<unsigned int>(experimentValue);
        auto console = std::make_shared<spdlog::logger>("console", std::make_shared<spdlog::sinks::null_sink_mt>());

        universe = std::make_shared<UniverseType>(settings, console);
        universe->initBodies(std::make_shared<UniverseShape>());
    }

    // Returns vector of experimentValues, nrIterations.
    std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override {
        std::vector<std::pair<int64_t, uint64_t>> problemSpace;

        const int totalNumberOfTests = 16;
        auto getNrBodies = [&](unsigned i) {  // Must never return 0.
            return std::pow(2, i+5);
        };

        /*auto getNrIterations = [&](unsigned i) {  // Must return MinIterations if i == totalNumberOfTests
            return MinIterations * ComplexityScaler()(getNrBodies(totalNumberOfTests - 1)) /
                   ComplexityScaler()(getNrBodies(i));
        };*/

        for (unsigned i = 0; i < totalNumberOfTests; ++i) {
            //problemSpace.emplace_back(getNrBodies(i), getNrIterations(i));
            problemSpace.emplace_back(getNrBodies(i), getNrIterationss(getNrBodies(i)));
        }

        return problemSpace;
    };

    uint64_t getNrIterationss(const int64_t experimentValue) const {
        // The smallest test should take at least 10x as long as our timer's resolution.
        // I chose "10x" arbitrarily.
        const auto minTestTime = static_cast<int64_t>(celero::timer::CachePerformanceFrequency(true) * 1e6) * 10;

        // Compute a good number to use for iterations and set the sample size to 30.
        auto testTime = int64_t(0);
        auto testIterations = uint64_t(1);

        UniverseFixture copy = *this;
        while (testTime < minTestTime) {
            testTime = copy.run(1, testIterations, experimentValue);
            if (testTime < minTestTime) {
                testIterations *= 2;
            }
        }
        const auto iterations = static_cast<uint64_t>(std::max(static_cast<double>(testIterations), 1000000.0 / testTime));
        return iterations;
    }

    std::shared_ptr<AbstractUniverse> universe;
    unsigned nrIterations;
protected:
    void UserBenchmark() override {
        universe->calcNextPosition();
    }
};

struct LinearScaler {
    double operator()(double x) {
        return x;
    }
};

struct QuadraticScaler {
    double operator()(double x) {
        return x * x;
    }
};

struct NLog2NScaler {
    double operator()(double x) {
        return x * log2(x);
    }
};

typedef UniverseFixture<BruteForceUniverse, RandomCubeUniverseShape, 10, LinearScaler> BruteForceSingleThread;
BASELINE_F(CalcNextPosition, BruteForceSingleThread, BruteForceSingleThread, 0, 0) {
    universe->calcNextPosition();
}

//typedef UniverseFixture<BruteForceMultiThreadUniverse, RandomCubeUniverseShape, 10, LinearScaler> BruteForceMultiThread;
//BENCHMARK_F(CalcNextPosition, BruteForceMultiThread, BruteForceMultiThread, 0, 0) {
//    universe->calcNextPosition();
//}

typedef UniverseFixture<BruteForceOffloadUniverse3, RandomCubeUniverseShape, 10, LinearScaler> BruteForceOffload;
BENCHMARK_F(CalcNextPosition, BruteForceOffload, BruteForceOffload, 0, 0) {
    universe->calcNextPosition();
}

//typedef UniverseFixture<BarnesHutTreeUniverse, RandomCubeUniverseShape, 10, LinearScaler> BarnesHutSingleThread;
//BENCHMARK_F(CalcNextPosition, BarnesHutSingleThread, BarnesHutSingleThread, 0, 0) {
//    universe->calcNextPosition();
//}
