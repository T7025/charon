//
// Created by thomas on 3/13/18.
//

#ifndef CHARON_ABSTRACTUNIVERSE_H
#define CHARON_ABSTRACTUNIVERSE_H

#include <spdlog/spdlog.h>
#include <boost/property_tree/ptree.hpp>
#include <settings/Settings.h>
#include <universeShapes/UniverseShape.h>
#include "Util.h"
#include "Vector3.h"

//class AbstractUniverse {};
//BinaryShape()
//template <typename UniverseShape>
class AbstractUniverse {
public:
    AbstractUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    virtual ~AbstractUniverse() = default;

    virtual void initBodies(std::shared_ptr<UniverseShape> shape);

    virtual void addBody(fp mass, Vector3 position, Vector3 velocity, Vector3 acceleration) = 0;
    virtual void addBody(fp mass, Vector3 position, Vector3 velocity);

    virtual size_t bodyCount() const = 0;

    virtual void calculateFirstStep() = 0;

    virtual void calculateNextStep() = 0;

    virtual void logInternalState() const = 0;

    // Used for testing
    virtual std::vector<Vector3> getPositions() const = 0;

    virtual fp calcEnergy() const = 0;

    virtual Vector3 calcMomentum() const = 0;

    virtual void calcNextPosition() = 0;
protected:
    const fp timeStep;
    //std::shared_ptr<UniverseShape> initialShape;
    std::shared_ptr<Settings> settings;
    std::shared_ptr<spdlog::logger> console;
};


#endif //CHARON_ABSTRACTUNIVERSE_H
