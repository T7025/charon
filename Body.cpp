//
// Created by thomas on 2/18/18.
//

#include "Body.h"

Body::Body(const Position &pos, const Velocity &vel) : position{pos}, velocity{vel} {

}

std::pair<Position, double> Body::calcCenterOfMass(Body other) {
    double newMass = mass + other.mass;
    Position newPos = (position * newMass + other.position * newMass) / newMass;
    return std::make_pair(newPos, newMass);
}
