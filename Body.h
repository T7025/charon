//
// Created by thomas on 2/18/18.
//

#ifndef CHARON_BODY_H
#define CHARON_BODY_H


#include <utility>
#include "Position.h"
#include "Velocity.h"


class Body {
public:
    Body(const Position &pos, const Velocity &vel);

    std::pair<Position, double> calcCenterOfMass(Body body);

private:
    Position position;
    Velocity velocity;
    double mass;
};


#endif //CHARON_BODY_H
