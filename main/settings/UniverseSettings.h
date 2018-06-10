//
// Created by thomas on 3/17/18.
//

#ifndef CHARON_UNIVERSESETTINGS_H
#define CHARON_UNIVERSESETTINGS_H


#include <base/Util.h>
#include <random>
#include <boost/property_tree/ptree.hpp>

struct UniverseSettings {
    unsigned long rngSeed;
    fp timeStep;
    fp gravitationalConstant;
    bool useCollisions;
    fp collisionRadius;
    unsigned number_bodies;

    UniverseSettings() = default;
    UniverseSettings(std::shared_ptr<boost::property_tree::ptree> ptree) :
            rngSeed{ptree->get("rng_seed", std::random_device{}())},
            timeStep{ptree->get<fp>("time_step")},
            gravitationalConstant{ptree->get("gravitational_constant", fp{1.0})},
            useCollisions{ptree->get("use_collisions", false)},
            collisionRadius{ptree->get("collision_radius", fp{0})},
            number_bodies{ptree->get("number_bodies", 0u)} {}
};


#endif //CHARON_UNIVERSESETTINGS_H
