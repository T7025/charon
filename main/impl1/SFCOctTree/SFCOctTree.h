//
// Created by thomas on 6/2/18.
//

#ifndef CHARON_SFCOCTTREE_H
#define CHARON_SFCOCTTREE_H


#include <vector>
#include <memory>
#include "Node.h"

namespace spdlog {
class logger;
}

//#pragma omp declare target
class SFCOctTree {
public:
    SFCOctTree(const Vector3 pos[], const Vector3 vel[], const Vector3 acc[], const fp mass[], const unsigned size);
    SFCOctTree();
    ~SFCOctTree();

    void addBody(fp mass, const Vector3 &pos, const Vector3 &vel, const Vector3 &acc);

    void initBodies();

    size_t size() const;

    void buildTree();

    Vector3 calculateAcceleration(const Vector3 &position) const;

    void calcNextPosition(const fp timeStep);

    void calculateFirstAcceleration(const fp timeStep);

    void calculateAcceleration(const fp timeStep);

    void logInternalState(const std::shared_ptr<spdlog::logger> &log) const;

    std::vector<Vector3> getPositions() const;

private:
    std::vector<Node> tree;
    Vector3 treeBoundingBox;

    Vector3 scalePositions();

    void calcSFCIndices(const Vector3 &spaceSize);

    void sortTree();

    void generateInternalNodes();

    void removeDuplicateInternalNodes();

    void establishParentChildRel();

    void calculateNodeData();

    friend std::ostream &operator<<(std::ostream &out, const SFCOctTree &tree);
};

std::ostream &operator<<(std::ostream &out, const SFCOctTree &tree);


#endif //CHARON_SFCOCTTREE_H
