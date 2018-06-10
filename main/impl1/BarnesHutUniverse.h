//
// Created by thomas on 4/26/18.
//

#ifndef CHARON_BARNESHUTUNIVERSE_H
#define CHARON_BARNESHUTUNIVERSE_H


#include <base/AbstractUniverse.h>
#include "OctTreeNode.h"

class BarnesHutUniverse : public AbstractUniverse {
public:
    BarnesHutUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    ~BarnesHutUniverse() override = default;

    void addBody(fp mass, Vector3 position, Vector3 velocity, Vector3 acceleration) override;

    size_t bodyCount() const override;

    void calculateFirstStep() override;

    void calculateNextStep() override;

    void logInternalState() const override;

// Used for testing
    std::vector<Vector3> getPositions() const override;

    fp calcEnergy() const override;

    Vector3 calcMomentum() const override;

    void calcNextPosition();
private:
    std::vector<fp> mass;
    std::vector<Vector3> position;
    std::vector<Vector3> velocity;
    std::vector<Vector3> acceleration;

    std::unique_ptr<OctTree> octTree;


    void fillTree();

    Vector3 calcAcceleration(const size_t target) const;
};


#endif //CHARON_BARNESHUTUNIVERSE_H
