//
// Created by thomas on 3/19/18.
//

#ifndef CHARON_UNIVERSESHAPES_H
#define CHARON_UNIVERSESHAPES_H


#include <universeShapes/UniverseShape.h>

struct UniverseShapes {
    std::shared_ptr<UniverseShape> operator()(const std::string &shapeName) const;
};


#endif //CHARON_UNIVERSESHAPES_H
