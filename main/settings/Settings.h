//
// Created by thomas on 3/16/18.
//

#ifndef CHARON_SETTINGS_H
#define CHARON_SETTINGS_H

#include <string>
#include <boost/property_tree/ptree.hpp>
#include "FileSettings.h"
#include "SimulatorSettings.h"
#include "UniverseSettings.h"

class Settings {
public:
    Settings();
    Settings(const std::string &configPath, const std::string &fileConfigName);

    std::shared_ptr<FileSettings> fileSettings;
    std::shared_ptr<SimulatorSettings> simulatorSettings;
    std::shared_ptr<UniverseSettings> universeSettings;

private:
    std::shared_ptr<boost::property_tree::ptree> pTree;
};

/*
struct IOSettings : public Settings {
    IOSettings(const std::string &filename);

    const std::string resultsDirPath;
    const std::string simulatorSettingsFilename;
    const std::string universeSettingsFilename;
};

struct SimulatorSettings : public Settings {
    SimulatorSettings(const std::string &filename);

    const unsigned nrIterations;
    const unsigned snapshotDelta;
};

struct UniverseSettings : public Settings {
    UniverseSettings(const std::string &filename);

    const unsigned long rngSeed;
    const fp timeStep;
    const fp gravitationalConstant;
    const bool useCollisions;
    const fp collisionRadius;
};*/

//struct Settings {
//    Settings(const std::string &configPath, const std::string &filenameIOSettings);
//
//    IOSettings ioSettings;
//    SimulatorSettings simulatorSettings;
//    UniverseSettings universeSettings;
//};


#endif //CHARON_SETTINGS_H
