//
// Created by thomas on 3/13/18.
//

#include <bruteForceImpl/BruteForceUniverse.h>
#include <universeShapes/BinaryUniverseShape.h>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <sstream>
#include <iomanip>
#include <simulator/Simulator.h>
#include <settings/Settings.h>
#include <iostream>
#include <omp.h>


int main() {
    //spdlog::set_pattern("[%l] [%H:%M:%S.%f] [thread %t] %v");
    // Setup logger
    spdlog::set_pattern("[%H:%M:%S.%f] [%l] %v");
    spdlog::set_async_mode(1u << 10u);
    auto console = spdlog::stdout_color_mt("console");

    // Setup config paths
    std::string configPrefix{"config"};
    configPrefix += '/';
    std::string defaultFileSettings{"charonConfig.json"};

    // Read settings
    auto settings = std::make_shared<Settings>(configPrefix, defaultFileSettings);

    // Create output directory
    boost::filesystem::path path(settings->fileSettings->resultsDirPath);
    if (!boost::filesystem::create_directories(path)) {
        console->critical("Could not create results directory. Exiting");
        return 1;
    }

    // Build simulator
    Simulator simulator{settings, console};
    // Build universe
    simulator.setup2();
//    simulator.setup<BruteForceUniverse, BinaryUniverseShape>();
    // Run simulation
    simulator.run();
}