//
// Created by thomas on 6/9/18.
//

#include "BarnesHutUniverse3.h"
#include <utility>

BarnesHutUniverse3::BarnesHutUniverse3(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console)
        : AbstractUniverse(std::move(settings), std::move(console)) {

}

void BarnesHutUniverse3::initBodies(std::shared_ptr<UniverseShape> shape) {
    AbstractUniverse::initBodies(shape);
}

void BarnesHutUniverse3::addBody(fp &&mass, Vector3 &&position, Vector3 &&velocity, Vector3 &&acceleration) {
    tree.addBody(mass, Vector3{position}, Vector3{velocity}, Vector3{acceleration});
}

size_t BarnesHutUniverse3::bodyCount() const {
    return tree.size();
}

void BarnesHutUniverse3::calculateFirstStep() {
    tree.calculateFirstAcceleration(timeStep);
}

void BarnesHutUniverse3::calculateNextStep() {
    tree.calcNextPosition(timeStep);
    tree.calculateAcceleration(timeStep);
}

void BarnesHutUniverse3::logInternalState() const {
    console->info("{} bodies", bodyCount());
    spdlog::drop("internalStateLogger");

    static int fileCount = 0;

    std::ostringstream fileName;
    fileName << settings->fileSettings->resultsDirPath
             << "/" << settings->fileSettings->resultsFilenamePrefix << fileCount++ << ".csv";

    auto fileLogger = spdlog::basic_logger_mt("internalStateLogger", fileName.str());
    fileLogger->set_pattern("%v");

    fileLogger->info("mass,xPos,yPos,zPos,xVel,yVel,zVel,xAcc,yAcc,zAcc");
    tree.logInternalState(fileLogger);
}

std::vector<Vector3> BarnesHutUniverse3::getPositions() const {
    return tree.getPositions();
}

fp BarnesHutUniverse3::calcEnergy() const {
    return 0;
}

Vector3 BarnesHutUniverse3::calcMomentum() const {
    return {};
}

void BarnesHutUniverse3::calcNextPosition() {
    tree.calcNextPosition(timeStep);
}

