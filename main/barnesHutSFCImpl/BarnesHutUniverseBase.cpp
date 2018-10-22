//
// Created by thomas on 6/14/18.
//

#include "BarnesHutUniverseBase.h"

BarnesHutUniverseBase::BarnesHutUniverseBase(std::shared_ptr<Settings> settings,
                                             std::shared_ptr<spdlog::logger> console)
        : AbstractUniverse(settings, console), errorRate{this->settings->simulatorSettings->barnesHutCutoff} {}

void BarnesHutUniverseBase::initBodies(std::shared_ptr<UniverseShape> shape) {
    AbstractUniverse::initBodies(shape);
    numBodies = static_cast<unsigned int>(tree.size());
    tree.reserve(4 * numBodies - 3);
}

void BarnesHutUniverseBase::addBody(fp &&mass, Vector3 &&position, Vector3 &&velocity, Vector3 &&acceleration) {
    tree.emplace_back(position, velocity, acceleration, mass);
}

size_t BarnesHutUniverseBase::bodyCount() const {
    return numBodies;
}

void BarnesHutUniverseBase::logInternalState() const {
    assert(!treeIsBuilt);

    console->info("{} bodies", bodyCount());
    spdlog::drop("internalStateLogger");

    static int fileCount = 0;

    std::ostringstream fileName;
    fileName << settings->fileSettings->resultsDirPath
             << "/" << settings->fileSettings->resultsFilenamePrefix << fileCount++ << ".csv";

    auto fileLogger = spdlog::basic_logger_mt("internalStateLogger", fileName.str());
    fileLogger->set_pattern("%v");

    fileLogger->info("mass,xPos,yPos,zPos,xVel,yVel,zVel,xAcc,yAcc,zAcc");
    for (unsigned i = 0; i < bodyCount(); ++i) {
        if (tree[i].isLeaf()) {
            fileLogger->info(
                    "{},{},{},{}",
                    tree[i].getMass(), tree[i].getPosition(), tree[i].getVelocity(), tree[i].getAcceleration());
        }
    }
}

std::vector<Vector3> BarnesHutUniverseBase::getPositions() const {
    assert(!treeIsBuilt);
    std::vector<Vector3> result{};
    const auto size = bodyCount();
    for (unsigned i = 0; i < size; ++i) {
        result.push_back(tree[i].getPosition());
    }
    return result;}

fp BarnesHutUniverseBase::calcEnergy() const {
    assert(!treeIsBuilt);
    fp result = 0;
    const auto size = bodyCount();
    for (unsigned i = 0; i < size; ++i) {
        auto velNorm = tree[i].getVelocity().norm();
        result += tree[i].getMass() * velNorm * velNorm / 2;
    }
    return result;
}

Vector3 BarnesHutUniverseBase::calcMomentum() const {
    assert(!treeIsBuilt);

    Vector3 result = {0,0,0};
    const auto size = bodyCount();
    for (unsigned i = 0; i < size; ++i) {
        result += tree[i].getVelocity() * tree[i].getMass();
    }
    return result;
}

std::ostream &operator<<(std::ostream &out, const BarnesHutUniverseBase &tree) {
    std::string tabSpace = "'";
    for (const auto &node : tree.tree) {
        std::string tab;
        for (unsigned i = 0; i < node.getDepth(); ++i) {
            tab += tabSpace;
        }
        out << tab << node << "\n";
    }
    return out;
}