//
// Created by thomas on 2/22/18.
//

#include "Simulator2.h"

Simulator2::Simulator2(std::ostream &out) : out{out} {

}

void Simulator2::setupAccelerations() {

}

void Simulator2::run() {

}

double Simulator2::distance(Body lhs, Body rhs) {
    double sqrtResult = sqrtDistance(lhs, rhs);
    return sqrtResult * sqrtResult;
}

double Simulator2::sqrtDistance(Body lhs, Body rhs) {
    return (xPositions[lhs] - xPositions[rhs]) * (xPositions[lhs] - xPositions[rhs])
           + (yPositions[lhs] - yPositions[rhs]) * (yPositions[lhs] - yPositions[rhs])
           + (zPositions[lhs] - zPositions[rhs]) * (zPositions[lhs] - zPositions[rhs]);
}
