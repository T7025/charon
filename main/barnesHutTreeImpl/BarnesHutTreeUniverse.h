//
// Created by thomas on 4/26/18.
//

#ifndef CHARON_BARNESHUTTREEUNIVERSE_H
#define CHARON_BARNESHUTTREEUNIVERSE_H


#include <base/AbstractUniverse.h>
#include "OctTreeNode.h"

class BarnesHutTreeUniverse : public AbstractUniverse {
public:
    BarnesHutTreeUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    ~BarnesHutTreeUniverse() override = default;

    void addBody(fp &&mass, Vector3 &&position, Vector3 &&velocity, Vector3 &&acceleration) override;

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
    const fp errorRate;

    void fillTree();

    Vector3 calcAcceleration(const size_t target) const;
};


#endif //CHARON_BARNESHUTTREEUNIVERSE_H
