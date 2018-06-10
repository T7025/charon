//
// Created by thomas on 2/22/18.
//

#ifndef CHARON_SIMULATOR2_H
#define CHARON_SIMULATOR2_H


#include <vector>
#include <iostream>
#include "Position.h"

using Body = ulong;

class Simulator2 {
public:
    Simulator2(std::ostream &out);

    void setupAccelerations();

    void run();


private:
    std::ostream &out;

    std::vector<Position> positions;


    std::vector<double> xPositions;
    std::vector<double> yPositions;
    std::vector<double> zPositions;

    std::vector<double> xVelocities;
    std::vector<double> yVelocities;
    std::vector<double> zVelocities;

    std::vector<double> xAccelerations;
    std::vector<double> yAccelerations;
    std::vector<double> zAccelerations;


    double distance(Body, Body);
    double sqrtDistance(Body, Body);

};


#endif //CHARON_SIMULATOR2_H
