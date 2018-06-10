//
// Created by thomas on 2/19/18.
//

#ifndef CHARON_QUADTREE_H
#define CHARON_QUADTREE_H


#include <vector>
#include <memory>
#include "Body.h"
#include "Universe.h"


class QuadTree {
public:
    QuadTree(const Universe &universe);

    QuadTree();

    void insert(const std::shared_ptr<Body> newBody);



private:
    void createChildren();
    bool contains(const Position &position) const;


    std::array<std::unique_ptr<QuadTree>, 4> children;

    Position center;
    double radius;  // Half of box size
};


#endif //CHARON_QUADTREE_H
