//
// Created by thomas on 3/19/18.
//

#ifndef CHARON_RANDOMCUBEUNIVERSESHAPE_H
#define CHARON_RANDOMCUBEUNIVERSESHAPE_H


#include <universeShapes/UniverseShape.h>
#include <base/Vector3.h>
#include <random>

class RandomCubeUniverseShape : public UniverseShape {
public:
    void fill(AbstractUniverse *universe, unsigned nrBodies) override {
        fp radius = 10.0;  // radius of the universe

        std::mt19937 engine{45};
        std::normal_distribution<fp> massDistribution{1.0, 1.0/3.0};  // Solar mass
        std::uniform_real_distribution<fp> positionDistr{-radius, radius};
        std::normal_distribution<fp> velocityDistr{0, radius/50};
        for (unsigned i = 0; i < nrBodies; ++i) {
            fp mass{std::abs(massDistribution(engine))};
            Vector3 position{positionDistr(engine), positionDistr(engine), positionDistr(engine)};
            Vector3 velocity{velocityDistr(engine), velocityDistr(engine), velocityDistr(engine)};
            Vector3 acceleration{0, 0, 0};
            universe->addBody(mass, position, velocity, acceleration);
        }
    }
};


#endif //CHARON_RANDOMCUBEUNIVERSESHAPE_H
