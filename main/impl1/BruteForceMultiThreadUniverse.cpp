//
// Created by thomas on 5/8/18.
//

#include "BruteForceMultiThreadUniverse.h"
#include <boost/math/special_functions/pow.hpp>
#include <spdlog/fmt/ostr.h>  // Needed to use ostream &operator<< overloads.
#include <sstream>
#include <iostream>

BruteForceMultiThreadUniverse::BruteForceMultiThreadUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console) :
        AbstractUniverse{std::move(settings), std::move(console)} {}

void BruteForceMultiThreadUniverse::addBody(fp mass, Vector3 position, Vector3 velocity, Vector3 acceleration) {
    this->mass.push_back(mass);
    this->position.push_back(position);
    this->velocity.push_back(velocity);
    this->acceleration.push_back(acceleration);
}
#pragma omp declare target
size_t BruteForceMultiThreadUniverse::bodyCount() const {
    return mass.size();
}
#pragma omp end declare target

Vector3 BruteForceMultiThreadUniverse::calcAcceleration(const unsigned int target) const {
    Vector3 newAcceleration{0, 0, 0};
    //#pragma omp declare target
//    auto lmass = this->mass;
//    auto lposition = this->position;
    //#pragma omp end declare target

    #pragma omp declare reduction(+: Vector3 : omp_out += omp_in)
//    #pragma omp target //map(tofrom:newAcceleration) map(to:lposition, lmass)
//    #pragma omp teams distribute parallel for simd //reduction(+:newAcceleration)
    #pragma omp parallel for reduction(+:newAcceleration)
    for (unsigned j = 0; j < bodyCount(); ++j) {
        if (target == j) continue;
        const Vector3 diff = position[j] - position[target];
        fp norm = diff.norm();
        newAcceleration += mass[j] * diff / boost::math::pow<3>(norm);
    }
    return newAcceleration;
}

void BruteForceMultiThreadUniverse::calcNextPosition() {
    const auto size = bodyCount();
    //#pragma omp simd
    #pragma omp parallel for
    for (unsigned i = 0; i < size; ++i) {
        position[i] += velocity[i] * timeStep + acceleration[i] * timeStep * timeStep / 2;
    }
}

void BruteForceMultiThreadUniverse::calculateFirstStep() {
    const auto size = bodyCount();
    for (unsigned i = 0; i < size; ++i) {
//        Vector3 newAcceleration = calcAcceleration(size, i);
//        acceleration[i] = newAcceleration;
    }
    calcNextPosition();
    for (unsigned i = 0; i < size; ++i) {
        Vector3 newAcceleration = calcAcceleration(i);
        velocity[i] += newAcceleration * timeStep;
        acceleration[i] = newAcceleration;
    }
    //for (unsigned i = 0; i < size; ++i) {
    //    std::cout << "pos: " << i << ": " << position[i].norm() << ",\t";
    //    std::cout << "vel: " << velocity[i].norm() << ",\t";
    //    std::cout << "acc: " << acceleration[i].norm() << ",\n";
    //}
    //std::cout << "momentum: " << calcMomentum() << "\n";
}


void BruteForceMultiThreadUniverse::calculateNextStep() {
    const auto size = bodyCount();

    calcNextPosition();
    for (unsigned i = 0; i < size; ++i) {
        Vector3 newAcceleration = calcAcceleration(i);
        velocity[i] += (acceleration[i] + newAcceleration) * timeStep / 2;
        acceleration[i] = newAcceleration;
    }
    //for (unsigned i = 0; i < size; ++i) {
    //    std::cout << "pos: " << i << ": " << position[i].norm() << ",\t";
    //    std::cout << "vel: " << velocity[i].norm() << ",\t";
    //    std::cout << "acc: " << acceleration[i].norm() << ",\n";
    //}
    //std::cout << "momentum: " << calcMomentum() << "\n";
}

void BruteForceMultiThreadUniverse::logInternalState() const {
    console->info("{} bodies", mass.size());
    spdlog::drop("internalStateLogger");

    static int fileCount = 0;

    std::ostringstream fileName;
    fileName << settings->fileSettings->resultsDirPath
             << "/" << settings->fileSettings->resultsFilenamePrefix << fileCount++ << ".csv";

    auto fileLogger = spdlog::basic_logger_mt("internalStateLogger", fileName.str());
    fileLogger->set_pattern("%v");

    fileLogger->info("mass,xPos,yPos,zPos,xVel,yVel,zVel,xAcc,yAcc,zAcc");
    for (unsigned i = 0; i < bodyCount(); ++i) {
        fileLogger->info("{},{},{},{}", mass[i], position[i], velocity[i], acceleration[i]);
    }
}

std::vector<Vector3> BruteForceMultiThreadUniverse::getPositions() const {
    return position;
}

fp BruteForceMultiThreadUniverse::calcEnergy() const {
    fp result = 0;
    for (unsigned i = 0; i < bodyCount(); ++i) {
        auto velNorm = velocity[i].norm();
        result += mass[i] * velNorm * velNorm / 2;
    }
    return result;
}

Vector3 BruteForceMultiThreadUniverse::calcMomentum() const {
    const auto size = bodyCount();
    Vector3 result = {0,0,0};
    for (unsigned i = 0; i < size; ++i) {
        result += mass[i] * velocity[i];
    }
    return result;
}