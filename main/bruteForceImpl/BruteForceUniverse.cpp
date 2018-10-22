//
// Created by thomas on 3/14/18.
//

#include "BruteForceUniverse.h"
#include <boost/math/special_functions/pow.hpp>
#include <spdlog/fmt/ostr.h>  // Needed to use ostream &operator<< overloads.
#include <sstream>
#include <iostream>

BruteForceUniverse::BruteForceUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console) :
        BruteForceUniverseBase{std::move(settings), std::move(console)} {}

Vector3 BruteForceUniverse::calcAcceleration(const unsigned int target) const {
    Vector3 newAcceleration{0, 0, 0};
    for (unsigned j = 0; j < bodyCount(); ++j) {
        if (target == j) continue;
        const Vector3 diff = position[j] - position[target];
        fp norm = diff.norm();
        newAcceleration += diff * (mass[j] / boost::math::pow<3>(norm));
    }
    return newAcceleration;
}

void BruteForceUniverse::calcNextPosition() {
    const auto size = bodyCount();
    for (unsigned i = 0; i < size; ++i) {
        position[i] += velocity[i] * timeStep + acceleration[i] * timeStep * timeStep / 2;
    }
}

void BruteForceUniverse::calculateFirstStep() {
    calcNextPosition();
    const auto size = bodyCount();
    for (unsigned i = 0; i < size; ++i) {
        Vector3 newAcceleration = calcAcceleration(i);
        velocity[i] += newAcceleration * timeStep;
        acceleration[i] = newAcceleration;
    }
    calcNextPosition();
}


void BruteForceUniverse::calculateNextStep() {
    const auto size = bodyCount();
    for (unsigned i = 0; i < size; ++i) {
        Vector3 newAcceleration = calcAcceleration(i);
        velocity[i] += (acceleration[i] + newAcceleration) * timeStep / 2;
        acceleration[i] = newAcceleration;
    }
    calcNextPosition();
}
