//
// Created by thomas on 2/19/18.
//

#include <cassert>
#include "QuadTree.h"


void QuadTree::insert(const std::shared_ptr<Body> newBody) {
    assert(contains(newBody->getPosition()) && "Body should be in bounds of this node");

/*
    if (!body) {
        body = newBody;
    }
    else if ()*/
}

void QuadTree::createChildren() {

}

bool QuadTree::contains(const Position &position) const {
//    position
//
//    return position.getX() - center.getX();
}
