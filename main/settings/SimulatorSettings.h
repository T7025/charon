//
// Created by thomas on 3/17/18.
//

#ifndef CHARON_SIMULATORSETTINGS_H
#define CHARON_SIMULATORSETTINGS_H


#include <boost/property_tree/ptree.hpp>

struct SimulatorSettings {
    unsigned nrIterations{};
    unsigned snapshotDelta{};
    std::string universe_type;
    std::string universe_shape;

    SimulatorSettings() = default;

    explicit SimulatorSettings(const std::shared_ptr<boost::property_tree::ptree> &ptree) :
            nrIterations{ptree->get<unsigned>("nr_iterations")},
            snapshotDelta{ptree->get("snapshot_delta", 10u)},
            universe_type{ptree->get<std::string>("universe_type")},
            universe_shape{ptree->get<std::string>("universe_shape")} {}
};


#endif //CHARON_SIMULATORSETTINGS_H
