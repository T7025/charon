//
// Created by thomas on 6/9/18.
//

#ifndef CHARON_BARNESHUTUNIVERSE3_H
#define CHARON_BARNESHUTUNIVERSE3_H


#include <base/AbstractUniverse.h>
#include <impl1/SFCOctTree/SFCOctTree.h>

class BarnesHutUniverse3 : public AbstractUniverse {
public:
    BarnesHutUniverse3(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    ~BarnesHutUniverse3() override = default;

    virtual void initBodies(std::shared_ptr<UniverseShape> shape);

    virtual void addBody(fp mass, Vector3 position, Vector3 velocity, Vector3 acceleration);

    virtual size_t bodyCount() const;

    virtual void calculateFirstStep();

    virtual void calculateNextStep();

    virtual void logInternalState() const;

    // Used for testing
    virtual std::vector<Vector3> getPositions() const;

    virtual fp calcEnergy() const;

    virtual Vector3 calcMomentum() const;

    virtual void calcNextPosition();

private:
    SFCOctTree tree;
};


#endif //CHARON_BARNESHUTUNIVERSE3_H
