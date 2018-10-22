//
// Created by thomas on 6/2/18.
//

#include "Node.h"

void TestPoint::test() {
    SFCIndex index{};
    bool res;
//    #pragma omp target //map(to: index) map(from: res)
    {
        res = true;//index.lessMSB(1, 2);
    }
    std::cout << res << std::endl;
}

//#pragma omp declare target
//const Vector3 &Node::getPosition() const {
//    return position;
//}
//
//Vector3 &Node::getPosition() {
//    return position;
//}
//
//const SFCIndex &Node::getSFCIndex() const {
//    return sfcIndex;
//}
//
//SFCIndex &Node::getSFCIndex() {
//    return sfcIndex;
//}
//
//bool Node::operator==(const Node &other) const {
//    return sfcIndex == other.sfcIndex && depth == other.depth;
//}
//
//const unsigned &Node::getDepth() const {
//    return depth;
//}
//
//unsigned &Node::getDepth() {
//    return depth;
//}
//
//const intv *Node::getChildren() const {
//    return data.children;
//}
//
//intv *Node::getChildren() {
//    return data.children;
//}
//
//const fp &Node::getMass() const {
//    return mass;
//}
//
//fp &Node::getMass() {
//    return mass;
//}
//
//const Vector3 &Node::getVelocity() const {
//    return data.leafData.vel;
//}
//
//Vector3 &Node::getVelocity() {
//    return data.leafData.vel;
//}
//
//const Vector3 &Node::getAcceleration() const {
//    return data.leafData.acc;
//}
//
//Vector3 &Node::getAcceleration() {
//    return data.leafData.acc;
//}
//
//bool Node::isLeaf() const {
//    return depth == k;
//}
//
//bool Node::isParentOf(const Node &other) const {
//    if (depth < other.depth) {
//        auto shift = k - depth + (sizeof(uintv) * 8 - k);
//        const auto isMax = static_cast<const unsigned>(shift == sizeof(uintv) * 8);
//        shift -= isMax;
//        if (((sfcIndex.x ^ other.sfcIndex.x) >> shift) >> isMax == 0
//            && ((sfcIndex.y ^ other.sfcIndex.y) >> shift) >> isMax == 0
//            && ((sfcIndex.z ^ other.sfcIndex.z) >> shift) >> isMax == 0) {
//            return true;
//        }
//    }
//    return false;
//}
//
//bool Node::operator<(const Node &other) const {
//    return sfcIndex < other.sfcIndex || (other.sfcIndex == sfcIndex && depth > other.depth);
//}
//#pragma omp end declare target

std::ostream &operator<<(std::ostream &out, const Node &node) {
    out << node.getSFCIndex() << "; " << node.getDepth() << "; " << node.getMass() << "; " << node.getPosition();
    if (node.getDepth() < k) {
        out << "; " << node.getChildren()[0];
        for (int i = 1; i < 8; ++i) {
            out << "," << node.getChildren()[i];
        }
    }
    return out;
}
