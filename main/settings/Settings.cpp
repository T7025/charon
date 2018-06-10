//
// Created by thomas on 3/16/18.
//

#include "Settings.h"
#include <boost/property_tree/json_parser.hpp>
#include <chrono>
#include <random>


Settings::Settings() :
        fileSettings{std::make_shared<FileSettings>()},
        simulatorSettings{std::make_shared<SimulatorSettings>()},
        universeSettings{std::make_shared<UniverseSettings>()} {}

Settings::Settings(const std::string &configPath, const std::string &fileConfigName) :
        pTree{std::make_shared<boost::property_tree::ptree>()} {
    // Read file settings
    boost::property_tree::json_parser::read_json(configPath + fileConfigName, *pTree);
    fileSettings = std::make_shared<FileSettings>(pTree);
    pTree->clear();
    // Read simulator settings
    boost::property_tree::json_parser::read_json(configPath + fileSettings->simulatorSettingsFilename, *pTree);
    simulatorSettings = std::make_shared<SimulatorSettings>(pTree);
    pTree->clear();
    // Read universe settings
    boost::property_tree::json_parser::read_json(configPath + fileSettings->universeSettingsFilename, *pTree);
    universeSettings = std::make_shared<UniverseSettings>(pTree);
    pTree->clear();
}

//IOSettings::IOSettings(const std::string &filename) :
//        Settings{filename},
//        resultsDirPath{[&]() {
//            std::string result = pTree->get("results_dir_prefix", "results");
//            if (result.back() != '/') {
//                result.push_back('/');
//            }
//            std::ostringstream pathName;
//            pathName << result << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//            return pathName.str();
//        }()},
//        //simulatorSettingsFilename{pTree->get("universe_config", "universeConfig.json")},
//        simulatorSettingsFilename{pTree->get<std::string>("simulator_config")},
//        universeSettingsFilename{pTree->get<std::string>("universe_config")} { pTree.reset(); }
//
//SimulatorSettings::SimulatorSettings(const std::string &filename) :
//        Settings{filename},
//        nrIterations{pTree->get<unsigned>("nr_iterations")},
//        snapshotDelta{pTree->get("snapshot_delta", 10u)} { pTree.reset(); }
//
//UniverseSettings::UniverseSettings(const std::string &filename) :
//        Settings{filename},
//        rngSeed{pTree->get("rng_seed", std::random_device{}())},
//        timeStep{pTree->get<fp>("time_step")},
//        gravitationalConstant{pTree->get("gravitational_constant", fp{1.0})},
//        useCollisions{pTree->get("use_collisions", false)},
//        collisionRadius{pTree->get("collision_radius", fp{0})} { pTree.reset(); }
