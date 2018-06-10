//
// Created by thomas on 3/15/18.
//

#ifndef CHARON_UNIVERSESHAPE_H
#define CHARON_UNIVERSESHAPE_H


#include <memory>
class AbstractUniverse;

class UniverseShape {
public:
    virtual ~UniverseShape() = default;

    void fill(const std::shared_ptr<AbstractUniverse> &universe, unsigned nrBodies) {
        fill(universe.get(), nrBodies);
    };
    virtual void fill(AbstractUniverse *universe, unsigned nrBodies) = 0;
};


#endif //CHARON_UNIVERSESHAPE_H
