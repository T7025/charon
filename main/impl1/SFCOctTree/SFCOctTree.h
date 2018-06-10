//
// Created by thomas on 6/2/18.
//

#ifndef CHARON_SFCOCTTREE_H
#define CHARON_SFCOCTTREE_H


#include <vector>
#include "Node.h"

class SFCOctTree {
public:
    SFCOctTree(const Vector3Offload pos[], const Vector3Offload vel[], const Vector3Offload acc[], const fp mass[], const unsigned size);
    ~SFCOctTree();

    void buildTree();

    void calculateNodeData();

    Vector3Offload calculateAcceleration(const Vector3Offload &position) const;

    void calcNextPosition(const fp timeStep);

private:
    std::vector<Node> tree;
    Vector3Offload treeBoundingBox;

    Vector3Offload scalePositions();

    void calcSFCIndices(const Vector3Offload &spaceSize);

    void sortTree();

    void generateInternalNodes();

    void removeDuplicateInternalNodes();

    void establishParentChildRel();

    friend std::ostream &operator<<(std::ostream &out, const SFCOctTree &tree);
};

std::ostream &operator<<(std::ostream &out, const SFCOctTree &tree);


#endif //CHARON_SFCOCTTREE_H
