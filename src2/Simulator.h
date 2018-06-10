//
// Created by thomas on 2/22/18.
//

#ifndef CHARON_SIMULATOR_H
#define CHARON_SIMULATOR_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include "Position.h"
#include "Velocity.h"
#include "Body.h"

static constexpr double gravConstant = 6.674 * pow(10, -11);


class Simulator {
public:
    Simulator(std::fstream &out) : out{out} {}

    void run() {
        // Calculate acceleration of bodies first time
        accelerationBody = {0, 0};
        for (auto &staticBody :  staticBodies) {
            accelerationBody += calcAcceleration(position, staticBody.getPosition(), staticBody.getMass());
        }
        for (int i = 0; i < 1000; ++i) {
            out << position << "\n";
            std::cout << "Pos: " << position << ",\tVel: " << velocity << ",\tTotVel: " << velocity.norm() << std::endl;

            // Move all bodies
            dynamicBody.getPosition() = position + velocity * timeStep + accelerationBody / 2 * timeStep * timeStep;

            // Calculate new acceleration
            Vector2 newAcceleration{0, 0};
            for (auto &staticBody : staticBodies) {
                newAcceleration += calcAcceleration(position, staticBody.getPosition(), staticBody.getMass());
            }

            std::cout << newAcceleration << std::endl;
            // Update velocity
            velocity = velocity + (accelerationBody + newAcceleration) / 2 * timeStep;
            accelerationBody = newAcceleration;
        }
    }

    Vector2 calcAcceleration(const Position &position1, const Position &position2, double mass2) const {
        Position delta = (position2 - position1).normalize();
        return delta * mass2 / position1.distance2(position2);
    }

private:
    Body dynamicBody = {{10, 0}, {0,0}, 10.0};
    Vector2 accelerationBody;
    std::vector<Body> staticBodies{
        {{0, 10}, {0,0}, 10.0},
        {{0, -10}, {0,0}, 10.0}
    };


    double mass = 10.0;
    Position position{10.0, 0.0};

    Velocity velocity{0.0, sqrt(10.0 / position.distance({0,0}))};

    double timeStep = 0.2;
    std::fstream &out;
};


#endif //CHARON_SIMULATOR_H
