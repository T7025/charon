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

    void initBodies(std::shared_ptr<UniverseShape> shape) override;

    void addBody(fp &&mass, Vector3 &&position, Vector3 &&velocity, Vector3 &&acceleration) override;

    size_t bodyCount() const override;

    void calculateFirstStep() override;

    void calculateNextStep() override;

    void logInternalState() const override;

    // Used for testing
    std::vector<Vector3> getPositions() const override;

    fp calcEnergy() const override;

    Vector3 calcMomentum() const override;

    void calcNextPosition() override;

private:
    SFCOctTree tree;
};


#endif //CHARON_BARNESHUTUNIVERSE3_H
