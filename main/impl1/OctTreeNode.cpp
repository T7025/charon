//
// Created by thomas on 4/26/18.
//

#include <boost/math/special_functions/pow.hpp>
#include "OctTreeNode.h"

void OctTreeNode::addBody(const fp mass, const Vector3 &bodyPosition, Vector3 nodeCenter, fp radius) {
    if (totalMass == 0) {
        totalMass = mass;
        centerOfMass = bodyPosition;
        this->nodeCenter = nodeCenter;
        this->radius = radius;
    } else {
        if (bodyPosition == centerOfMass) {
            throw std::runtime_error{"infinite recursion: new body position equals current center of mass"};
        }
        fp halfRadius = radius / 2;
        if (children.empty()) {
            children.resize(8);
            /*for (const auto &child : children) {
                std::cout << child.centerOfMass << "|";
            }
            std::cout << "\n";*/

            const auto childNodeCenter = getChildNodeCenter(this->nodeCenter, radius, centerOfMass);
            const auto childIndex = getChildNodeIndex(centerOfMass);
            children[childIndex].addBody(totalMass, centerOfMass, childNodeCenter, halfRadius);
        }
        const auto childNodeCenter = getChildNodeCenter(this->nodeCenter, radius, bodyPosition);
        const auto childIndex = getChildNodeIndex(bodyPosition);
        children[childIndex].addBody(mass, bodyPosition, childNodeCenter, halfRadius);

        const auto newTotalMass = totalMass + mass;
        centerOfMass = (centerOfMass * totalMass + bodyPosition * mass) / newTotalMass;
        totalMass = newTotalMass;
    }
}

Vector3 OctTreeNode::getChildNodeCenter(Vector3 curNodeCenter, fp curNodeRadius, const Vector3 &newBodyPosition) const {
    fp halfRadius = curNodeRadius / 2;
    if (newBodyPosition.x < this->nodeCenter.x) {
        curNodeCenter.x -= halfRadius;
    } else {
        curNodeCenter.x += halfRadius;
    }
    if (newBodyPosition.y < this->nodeCenter.y) {
        curNodeCenter.y -= halfRadius;
    } else {
        curNodeCenter.y += halfRadius;
    }
    if (newBodyPosition.z < this->nodeCenter.z) {
        curNodeCenter.z -= halfRadius;
    } else {
        curNodeCenter.z += halfRadius;
    }
    return curNodeCenter;
}

size_t OctTreeNode::getChildNodeIndex(const Vector3 &newBodyPosition) const {
    unsigned childIndex = 0;
    childIndex |=  (newBodyPosition.x < this->nodeCenter.x) << 2u;
    childIndex |=  (newBodyPosition.y < this->nodeCenter.y) << 1u;
    childIndex |=  (newBodyPosition.z < this->nodeCenter.z) << 0u;
    return childIndex;
}

std::ostream &OctTreeNode::printTree(std::ostream &out, unsigned depth) const {
    for (unsigned i = 0; i < depth; ++i) {
        out << "  ";
    }
    out << "totMass: " << totalMass << ", CoM: " << centerOfMass
        << ", nodeCenter: " << nodeCenter << ", radius: " << radius << "\n";
    for (const auto &child : children) {
        child.printTree(out, depth + 1);
    }
    return out;
}

void OctTreeNode::calculateAcceleration(Vector3 &result, const Vector3 &targetPosition) const {
    const fp errorRate = 0.5;
    // if node is self
    //std::cout << centerOfMass << "\n";
    if (centerOfMass == targetPosition) {
        return;
    }
    // if leaf node
    else if (children.empty() || radius * radius / centerOfMass.squareDistance(targetPosition) < errorRate * errorRate) {
        const Vector3 diff = centerOfMass - targetPosition;
        const fp norm = diff.norm();
        result += totalMass * diff / boost::math::pow<3>(norm);
        return;
    }
    for (const auto &child : children) {
        child.calculateAcceleration(result, targetPosition);
    }
}

OctTree::OctTree(const Vector3 &treeCenter, fp radius) : treeCenter{treeCenter}, radius{radius} {}

void OctTree::addBody(fp mass, const Vector3 &bodyPosition) {
    root.addBody(mass, bodyPosition, treeCenter, radius);
}

std::ostream &OctTree::printTree(std::ostream &out) const {
    root.printTree(out, 0);
    return out;
}

Vector3 OctTree::calculateAcceleration(const Vector3 &targetPosition) const {
    Vector3 result{0,0,0};
    root.calculateAcceleration(result, targetPosition);
    return result;
}
