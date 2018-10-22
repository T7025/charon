//
// Created by thomas on 4/26/18.
//

#ifndef CHARON_OCTTREE_H
#define CHARON_OCTTREE_H


#include <vector>
#include <base/Vector3.h>
#include <iostream>

class OctTreeNode {
public:
    OctTreeNode() = default;

    void addBody(fp mass, const Vector3 &bodyPosition, Vector3 nodeCenter, fp radius);

    Vector3 getChildNodeCenter(Vector3 curNodeCenter, fp curNodeRadius, const Vector3 &newBodyPosition) const;

    size_t getChildNodeIndex(const Vector3 &newBodyPosition) const;

    std::ostream &printTree(std::ostream &out, unsigned depth) const;

    void calculateAcceleration(Vector3 &result, const Vector3 &targetPosition, const fp &errorRate) const;
    fp totalMass = 0;
private:
    // Body info
    Vector3 centerOfMass = {0,0,0};

    // Node info
    fp radius = 0;
    Vector3 nodeCenter = {0,0,0};
    std::vector<OctTreeNode> children;
};

class OctTree {
public:
    OctTree(const Vector3 &treeCenter, fp radius);

    void addBody(fp mass, const Vector3 &bodyPosition);

    std::ostream &printTree(std::ostream &out) const;

    Vector3 calculateAcceleration(const Vector3 &targetPosition, const fp &errorRate) const;

private:
    Vector3 treeCenter;
    fp radius;
    OctTreeNode root;
};


#endif //CHARON_OCTTREE_H
