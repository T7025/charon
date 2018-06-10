//
// Created by thomas on 3/17/18.
//

#ifndef CHARON_FILESETTINGS_H
#define CHARON_FILESETTINGS_H


#include <string>
#include <boost/property_tree/ptree.hpp>
#include <chrono>

struct FileSettings {
    bool enableFileOutput;
    std::string resultsDirPath;
    std::string resultsFilenamePrefix;
    std::string simulatorSettingsFilename;
    std::string universeSettingsFilename;

    FileSettings() = default;
    FileSettings(std::shared_ptr<boost::property_tree::ptree> ptree) :
            enableFileOutput{ptree->get("enable_file_output", true)},
            resultsDirPath{[&]() {
                std::string result = ptree->get("results_dir_prefix", "results");
                if (result.back() != '/') {
                    result.push_back('/');
                }
                std::ostringstream pathName;
                pathName << result << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                return pathName.str();
            }()},
            resultsFilenamePrefix{ptree->get("results_filename_prefix", "out")},
            simulatorSettingsFilename{ptree->get<std::string>("simulator_config")},
            universeSettingsFilename{ptree->get<std::string>("universe_config")} {}
};

#endif //CHARON_FILESETTINGS_H
