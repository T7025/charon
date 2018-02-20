//
// Created by thomas on 2/18/18.
//

#include "Body.h"

Body::Body(const Position &pos, const Velocity &vel, double mass) : position{pos}, velocity{vel}, mass{mass} {

}

std::pair<Position, double> Body::calcCenterOfMass(Body other) const {
    double newMass = mass + other.mass;
    Position newPos = (position * mass + other.position * other.mass) / newMass;
    return std::make_pair(newPos, newMass);
}

Position Body::getPosition() const {
    return position;
}

Velocity Body::getVelocity() const {
    return velocity;
}

double Body::getMass() const {
    return mass;
}
