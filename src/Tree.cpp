//
// Created by thomas on 2/19/18.
//

#include "Tree.h"

Tree::Tree(Position corner, double size, unsigned bucketSize) : corner{corner}, size{size}, bucketSize{bucketSize} {
}

void Tree::insert(const std::shared_ptr<Body> newBody) {
    if (!contains(newBody->getPosition())) {
        return;
    }
    else if (children.empty()) {  // Is external node
        if (bodies.size() < bucketSize) {  // Add body to node
            bodies.push_back(newBody);
        } else {
            createChildren();

            for (const auto &body : bodies) {
                insert(body);
            }
            bodies.resize(0);
        }
    }
/*
    if (!body) {
        body = newBody;
    }
    else if ()*/
}

void Tree::createChildren() {
    double newSize = size / 2;

    children.emplace_back(corner + {0, 0}, newSize, bucketSize);
    children.emplace_back(corner + {0, newSize}, newSize, bucketSize);
    children.emplace_back(corner + {newSize, 0}, newSize, bucketSize);
    children.emplace_back(corner + {newSize, newSize}, newSize, bucketSize);
}

bool Tree::contains(const Position &position) const {
    // corner <= position < corner + size --> True
    auto diff = corner - position;
    return !(diff.getX() <= 0 || diff.getX() > size || diff.getY() <= 0 || diff.getY() > size);
}
