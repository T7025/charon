//
// Created by thomas on 6/14/18.
//

#ifndef CHARON_BARNESHUTUNIVERSEBASE_H
#define CHARON_BARNESHUTUNIVERSEBASE_H


#include <base/AbstractUniverse.h>
#include <impl1/SFCOctTree/Node.h>

class BarnesHutUniverseBase : public AbstractUniverse {
public:
    BarnesHutUniverseBase(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    void initBodies(std::shared_ptr<UniverseShape> shape) override;

    void addBody(fp &&mass, Vector3 &&position, Vector3 &&velocity, Vector3 &&acceleration) override;

    size_t bodyCount() const override;

    void logInternalState() const override;

    // Used for testing
    std::vector<Vector3> getPositions() const override;

    fp calcEnergy() const override;

    Vector3 calcMomentum() const override;

protected:
    std::vector<Node> tree;
    Vector3 treeBoundingBox;
    Vector3 offset;
    fp treeBoundingBoxSize;
    unsigned numBodies;
    const fp errorRate;
    bool treeIsBuilt = false;

    friend std::ostream &operator<<(std::ostream &out, const BarnesHutUniverseBase &tree);
};

std::ostream &operator<<(std::ostream &out, const BarnesHutUniverseBase &tree);

#endif //CHARON_BARNESHUTUNIVERSEBASE_H
