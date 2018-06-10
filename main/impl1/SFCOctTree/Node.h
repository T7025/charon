//
// Created by thomas on 6/2/18.
//

#ifndef CHARON_NODE_H
#define CHARON_NODE_H


#include <iostream>
#include <base/Vector3Offload.h>
#include "SFCIndex.h"

class TestPoint {
public:
    void test();
};

#pragma omp declare target
class Node {
public:
    Node() : sfcIndex{}, depth{}, mass{-1}, position{}, data{} {};
    Node(const Vector3Offload &pos, const Vector3Offload &vel, const Vector3Offload &acc, const fp mass) :
            sfcIndex{}, depth{k}, mass{mass}, position{pos}, data{vel, acc} {}
    const Vector3Offload &getPosition() const;
    Vector3Offload &getPosition();
    const SFCIndex &getSFCIndex() const;
    SFCIndex &getSFCIndex();
    const unsigned &getDepth() const;
    unsigned &getDepth();
    const intv *getChildren() const;
    intv *getChildren();
    const fp &getMass() const;
    fp &getMass();
    const Vector3Offload &getVelocity() const;
    Vector3Offload &getVelocity();
    const Vector3Offload &getAcceleration() const;
    Vector3Offload &getAcceleration();

    bool isLeaf() const;

    bool operator<(const Node &other) const;
    bool operator==(const Node &other) const;

private:
    SFCIndex sfcIndex;
    unsigned depth;
    fp mass;
    Vector3Offload position;

    union Data {
        Data() : children{-1,-1,-1,-1,-1,-1,-1,-1} {}
        Data(const Vector3Offload &vel, const Vector3Offload &acc) : leafData{vel, acc} {}
        intv children[8];
        struct {
            Vector3Offload vel;
            Vector3Offload acc;
        } leafData;
    } data;
};
#pragma omp end declare target

std::ostream &operator<<(std::ostream &out, const Node &node);

#endif //CHARON_NODE_H
