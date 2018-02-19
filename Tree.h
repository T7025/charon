//
// Created by thomas on 2/19/18.
//

#ifndef CHARON_TREE_H
#define CHARON_TREE_H


#include <vector>
#include <memory>
#include "Body.h"

struct Domain {
    double xMax, xMin, yMax, yMin;
};

class Tree {
public:
    Tree(Domain domain);

    void insert(std::shared_ptr<Body> body);

private:

    std::vector<std::unique_ptr<Tree>> children;
};


#endif //CHARON_TREE_H
