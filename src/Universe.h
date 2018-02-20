//
// Created by thomas on 2/18/18.
//

#ifndef CHARON_UNIVERSE_H
#define CHARON_UNIVERSE_H


#include <vector>
#include "Body.h"


enum class UniverseType {
    randomUniform
};

class Universe {
public:
    Universe();

    Universe(UniverseType);

private:
    std::vector<Body> bodies;
};


#endif //CHARON_UNIVERSE_H
