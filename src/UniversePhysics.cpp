//
// Created by thomas on 2/26/18.
//

#include "Universe.h"
#include "UniversePhysics.h"

BodyIndex UniversePhysics::mergeBodies(BodyIndex first, BodyIndex second) {
    Mass newMass = Universe::getMass(first) + Universe::getMass(second);
    Position newPos = (Universe::getPosition(first) * Universe::getMass(first) + Universe::getPosition(second) * Universe::getMass(second)) / newMass;
    Velocity newVel = (Universe::getVelocity(first) * Universe::getMass(first) + Universe::getVelocity(second) * Universe::getMass(second)) / newMass;
    Acceleration newAcc = (Universe::getAcceleration(first) * Universe::getMass(first) + Universe::getAcceleration(second) * Universe::getMass(second)) / newMass;
    
    Universe::setMass(first, newMass);
    Universe::setPosition(first, newPos);
    Universe::setVelocity(first, newVel);
    Universe::setAcceleration(first, newAcc);
    
    Universe::removeBody(second);
    return first;
}