//
// Created by thomas on 3/15/18.
//

#ifndef CHARON_BINARYUNIVERSESHAPE_H
#define CHARON_BINARYUNIVERSESHAPE_H

#include <universeShapes/UniverseShape.h>

class BinaryUniverseShape : public UniverseShape {
public:
    void fill(AbstractUniverse *universe, const unsigned) override {
        universe->addBody(1.0, {-1,0,0}, {0,-1.0/2,0});
        universe->addBody(1.0, {1,0,0}, {0,1.0/2,0});
    }
};

#endif //CHARON_BINARYUNIVERSESHAPE_H
