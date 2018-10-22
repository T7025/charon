//
// Created by thomas on 6/2/18.
//

#ifndef CHARON_NODE_H
#define CHARON_NODE_H


#include <iostream>
#include <base/Vector3.h>
#include "SFCIndex.h"

class TestPoint {
public:
    void test();
};

#pragma omp declare target
class Node {
public:
    Node() : sfcIndex{}, depth{}, mass{-1}, position{}, data{} {};
    Node(const Vector3 &pos, const Vector3 &vel, const Vector3 &acc, const fp mass) :
            sfcIndex{}, depth{k}, mass{mass}, position{pos}, data{vel, acc} {}
//    const Vector3 &getPosition() const;
//    Vector3 &getPosition();
//    const SFCIndex &getSFCIndex() const;
//    SFCIndex &getSFCIndex();
//    const unsigned &getDepth() const;
//    unsigned &getDepth();
//    const intv *getChildren() const;
//    intv *getChildren();
//    const fp &getMass() const;
//    fp &getMass();
//    const Vector3 &getVelocity() const;
//    Vector3 &getVelocity();
//    const Vector3 &getAcceleration() const;
//    Vector3 &getAcceleration();
//
//    bool isLeaf() const;
//    bool isParentOf(const Node &other) const;
//
//    bool operator<(const Node &other) const;
//    bool operator==(const Node &other) const;
    const Vector3 &getPosition() const {
        return position;
    }

    Vector3 &getPosition() {
        return position;
    }

    const SFCIndex &getSFCIndex() const {
        return sfcIndex;
    }

    SFCIndex &getSFCIndex() {
        return sfcIndex;
    }

    bool operator==(const Node &other) const {
        return sfcIndex == other.sfcIndex && depth == other.depth;
    }

    const unsigned &getDepth() const {
        return depth;
    }

    unsigned &getDepth() {
        return depth;
    }

    const intv *getChildren() const {
        return data.children;
    }

    intv *getChildren() {
        return data.children;
    }

    const fp &getMass() const {
        return mass;
    }

    fp &getMass() {
        return mass;
    }

    const Vector3 &getVelocity() const {
        return data.leafData.vel;
    }

    Vector3 &getVelocity() {
        return data.leafData.vel;
    }

    const Vector3 &getAcceleration() const {
        return data.leafData.acc;
    }

    Vector3 &getAcceleration() {
        return data.leafData.acc;
    }

    bool isLeaf() const {
        return depth == k;
    }

    bool isParentOf(const Node &other) const {
        if (depth < other.depth) {
            auto shift = k - depth + (sizeof(uintv) * 8 - k);
            const auto isMax = static_cast<const unsigned>(shift == sizeof(uintv) * 8);
            shift -= isMax;
            if (((sfcIndex.x ^ other.sfcIndex.x) >> shift) >> isMax == 0
                && ((sfcIndex.y ^ other.sfcIndex.y) >> shift) >> isMax == 0
                && ((sfcIndex.z ^ other.sfcIndex.z) >> shift) >> isMax == 0) {
                return true;
            }
        }
        return false;
    }

    bool operator<(const Node &other) const {
        return sfcIndex < other.sfcIndex || (other.sfcIndex == sfcIndex && depth > other.depth);
    }

private:
    SFCIndex sfcIndex;
    unsigned depth;
    fp mass;
    Vector3 position;

    union Data {
        Data() : children{-1,-1,-1,-1,-1,-1,-1,-1} {}
        Data(const Vector3 &vel, const Vector3 &acc) : leafData{vel, acc} {}
        intv children[8];
        struct {
            Vector3 vel;
            Vector3 acc;
        } leafData;
    } data;
};
#pragma omp end declare target

std::ostream &operator<<(std::ostream &out, const Node &node);

#endif //CHARON_NODE_H
