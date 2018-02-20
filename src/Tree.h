//
// Created by thomas on 2/19/18.
//

#ifndef CHARON_TREE_H
#define CHARON_TREE_H


#include <vector>
#include <memory>
#include "Body.h"


class Tree {
public:
    Tree(Position corner, double size, unsigned bucketSize = 1);

    void insert(const std::shared_ptr<Body> newBody);

private:
    void createChildren();
    bool contains(const Position &position) const;

    std::vector<std::shared_ptr<Body>> bodies;
    std::vector<std::unique_ptr<Tree>> children;

    Position corner;
    double size;

    unsigned bucketSize;
};


#endif //CHARON_TREE_H
