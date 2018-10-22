//
// Created by thomas on 6/17/18.
//

#include <bruteForceImpl/BruteForceUniverse.h>
#include <bruteForceImpl/BruteForceMultiThreadUniverse.h>
#include <spdlog/sinks/null_sink.h>
#include <bruteForceImpl/BruteForceOffloadUniverse3.h>
#include <barnesHutTreeImpl/BarnesHutTreeUniverse.h>
#include <barnesHutSFCImpl/BarnesHutUniverse.h>
#include <barnesHutSFCImpl/BarnesHutMultiThreadUniverse.h>
#include <base/Timer.h>
#include <universeShapes/RandomCubeUniverseShape.h>
#include <fstream>
#include <barnesHutSFCImpl/BarnesHutOffloadUniverse.h>

std::ofstream out{"benchmarks.csv"};

template <typename UniverseType>
void doBench(const std::string &benchName, int maxPower) {
    auto console = std::make_shared<spdlog::logger>("console", std::make_shared<spdlog::sinks::null_sink_mt>());
    auto settings = std::make_shared<Settings>();
    settings->fileSettings->enableFileOutput = false;
    settings->universeSettings->timeStep = 0.001;
    settings->universeSettings->gravitationalConstant = 1.0;
    settings->universeSettings->useCollisions = false;
    settings->simulatorSettings->barnesHutCutoff = 0.7;
    std::vector<unsigned> numBodiesVec;
    //for (int i = 6; i < 21; ++i) {
    for (int i = 6; i <= maxPower; ++i) {
        numBodiesVec.emplace_back(std::pow(2, i));
    }
    for (const auto &numBodies : numBodiesVec) {
        settings->universeSettings->number_bodies = numBodies;
        auto universe = std::make_shared<UniverseType>(settings, console);
        universe->initBodies(std::make_shared<RandomCubeUniverseShape>());

        double minTime = std::numeric_limits<double>::infinity();
        double avgTime = 0;
        int numRuns = 0;
        fp t = 0;
        for (int i = 0; (i < 30 || t < 5) && t < 60*15; ++i) {  // run at least 5 sec or 30 iterations, whichever takes longer as long as it stays under 15 min.
            auto start = omp_get_wtime();

            universe->calculateNextStep();

            auto end = omp_get_wtime();
            minTime = std::min(minTime, end - start);
            avgTime += end - start;
            numRuns++;
            t += end - start;
        }
        avgTime /= numRuns;
        std::cout << benchName <<","<< numRuns <<","<< numBodies <<","<< minTime <<","<<avgTime<<"\n";
        out << benchName <<","<< numRuns <<","<< numBodies <<","<< minTime <<","<<avgTime<<"\n";
    }
}

int main() {
    std::cout << "name,numRuns,numBodies,minTime,AvgTime\n";
    out << "name,numRuns,numBodies,minTime,AvgTime\n";
//    doBench<BarnesHutTreeUniverse>("BarnesHutTreeUniverse");
//    doBench<BarnesHutUniverse>("BarnesHutUniverse");

    for (int i = 2; i <= 6; ++i) {
        omp_set_num_threads(i);
        doBench<BarnesHutMultiThreadUniverse>("BarnesHutMultiThreadUniverse" + std::to_string(i), 20);
    }
//    doBench<BruteForceUniverse>("BruteForceUniverse");
    for (int i = 1; i <= 6; ++i) {
        omp_set_num_threads(i);
        doBench<BruteForceMultiThreadUniverse>("BruteForceMultiThreadUniverse" + std::to_string(i), 15);
    }
    doBench<BarnesHutOffloadUniverse>("BarnesHutOffloadUniverse", 18);
    doBench<BruteForceOffloadUniverse3>("BruteForceOffloadUniverse", 15);
    out.close();
}