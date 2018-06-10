//
// Created by thomas on 4/26/18.
//

#include "BarnesHutUniverse.h"
#include <spdlog/fmt/ostr.h>
#include <universeShapes/UniverseShape.h>
#include <utility>

BarnesHutUniverse::BarnesHutUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console)
        : AbstractUniverse(std::move(settings), std::move(console)) {
}

void BarnesHutUniverse::addBody(fp mass, Vector3 position, Vector3 velocity, Vector3 acceleration) {
    this->mass.push_back(mass);
    this->position.push_back(position);
    this->velocity.push_back(velocity);
    this->acceleration.push_back(acceleration);
}

size_t BarnesHutUniverse::bodyCount() const {
    return mass.size();
}

void BarnesHutUniverse::fillTree() {
    fp minX = std::numeric_limits<fp>::infinity();
    fp minY = std::numeric_limits<fp>::infinity();
    fp minZ = std::numeric_limits<fp>::infinity();

    fp maxX = -std::numeric_limits<fp>::infinity();
    fp maxY = -std::numeric_limits<fp>::infinity();
    fp maxZ = -std::numeric_limits<fp>::infinity();

    for (size_t i = 0; i < bodyCount(); ++i) {
        minX = std::min(minX, position[i].x);
        minY = std::min(minY, position[i].y);
        minZ = std::min(minZ, position[i].z);

        maxX = std::max(maxX, position[i].x);
        maxY = std::max(maxY, position[i].y);
        maxZ = std::max(maxZ, position[i].z);
    }

//    fp minMin = std::min(std::min(minX, minY), minZ);
//    fp maxMax = std::max(std::max(minX, minY), minZ);
    fp radius = std::max((maxX - minX), std::max(maxY - minY, maxZ - minZ));
//    radius = std::max((maxX - minX), std::max(maxY - minY, maxZ - minZ));
    //std::cout << minMin << ", " << maxMax << "\n";
    Vector3 center = {(maxX + minX), (maxY + minY), (maxZ + minZ)};
    center /= 2;

    octTree = std::make_unique<OctTree>(center, radius);

    for (size_t i = 0; i < bodyCount(); ++i) {
        //std::cout << "Adding: mass: " << mass[i] << ", pos: " << position[i] <<"\n";
        octTree->addBody(mass[i], position[i]);
        //octTree->printTree(std::cout);
        //std::cout <<"\n";
    }
}

Vector3 BarnesHutUniverse::calcAcceleration(const size_t target) const {
    return octTree->calculateAcceleration(position[target]);
}

void BarnesHutUniverse::calculateFirstStep() {
    calcNextPosition();
    fillTree();
    for (size_t i = 0; i < bodyCount(); ++i) {
        const Vector3 newAcceleration = calcAcceleration(i);
        velocity[i] += newAcceleration * timeStep;
        acceleration[i] = newAcceleration;
    }
}

void BarnesHutUniverse::calculateNextStep() {
    calcNextPosition();
    fillTree();
    for (unsigned i = 0; i < bodyCount(); ++i) {
        Vector3 newAcceleration = calcAcceleration(i);
        velocity[i] += (acceleration[i] + newAcceleration) * timeStep / 2;
        acceleration[i] = newAcceleration;
    }
}

void BarnesHutUniverse::logInternalState() const {
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

void BarnesHutUniverse::calcNextPosition() {
    for (unsigned i = 0; i < bodyCount(); ++i) {
        position[i] += velocity[i] * timeStep + acceleration[i] * timeStep * timeStep / 2;
    }
}


std::vector<Vector3> BarnesHutUniverse::getPositions() const {
    return position;
}

fp BarnesHutUniverse::calcEnergy() const {
    fp result = 0;
    for (unsigned i = 0; i < bodyCount(); ++i) {
        auto velNorm = velocity[i].norm();
        result += mass[i] * velNorm * velNorm / 2;
    }
    return result;
}

Vector3 BarnesHutUniverse::calcMomentum() const {
    const auto size = bodyCount();
    Vector3 result = {0,0,0};
    for (unsigned i = 0; i < size; ++i) {
        result += mass[i] * velocity[i];
    }
    return result;
}