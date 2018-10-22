//
// Created by thomas on 3/17/18.
//

#ifndef CHARON_SIMULATORSETTINGS_H
#define CHARON_SIMULATORSETTINGS_H


#include <boost/property_tree/ptree.hpp>
#include <base/Util.h>

struct SimulatorSettings {
    unsigned nrIterations{};
    unsigned snapshotDelta{};
    std::string universe_type;
    std::string universe_shape;
    fp barnesHutCutoff;

    SimulatorSettings() = default;

    explicit SimulatorSettings(const std::shared_ptr<boost::property_tree::ptree> &ptree) :
            nrIterations{ptree->get<unsigned>("nr_iterations")},
            snapshotDelta{ptree->get("snapshot_delta", 10u)},
            universe_type{ptree->get<std::string>("universe_type")},
            universe_shape{ptree->get<std::string>("universe_shape")},
            barnesHutCutoff{ptree->get("barnes_hut_cutoff", 0.7)} {}
};


#endif //CHARON_SIMULATORSETTINGS_H
