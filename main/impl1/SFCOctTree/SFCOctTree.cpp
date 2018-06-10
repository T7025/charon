//
// Created by thomas on 6/2/18.
//

#include "SFCOctTree.h"
#include <omp.h>
#include <limits>
#include <algorithm>
#include <cassert>

SFCOctTree::SFCOctTree(const Vector3Offload pos[], const Vector3Offload vel[], const Vector3Offload acc[],
                       const fp mass[], const unsigned size) {
    tree.resize(size);
    for (unsigned i = 0; i < size; ++i) {
        tree[i] = Node{pos[i], vel[i], acc[i], mass[i]};
    }
}

SFCOctTree::~SFCOctTree() {
    //delete(tree);
}

void SFCOctTree::buildTree() {
    // Scale positions so a corner of the bounding box is at (0,0,0); calculate size of bounding box.
    treeBoundingBox = scalePositions();
    std::cout << "Scaled positions" << std::endl;
    calcSFCIndices(treeBoundingBox);
    std::cout << "Calculated SFCIndices" << std::endl;

    for (unsigned i = 0; i < tree.size() - 1; ++i) {
        assert(!(tree[i] == tree[i+1]));
    }

    generateInternalNodes();
    std::cout << "Generated internal nodes" << std::endl;
    removeDuplicateInternalNodes();
    std::cout << "Removed duplicate internal nodes" << std::endl;
    establishParentChildRel();
    std::cout << "Established parent-child relation" << std::endl;
    calculateNodeData();
    std::cout << "Calculated node data" << std::endl;
}

Vector3Offload SFCOctTree::scalePositions() {
    Vector3Offload min = {
            std::numeric_limits<fp>::infinity(),
            std::numeric_limits<fp>::infinity(),
            std::numeric_limits<fp>::infinity()
    };
    Vector3Offload max = {
            -std::numeric_limits<fp>::infinity(),
            -std::numeric_limits<fp>::infinity(),
            -std::numeric_limits<fp>::infinity()
    };

    for (const auto &node : tree) {
        min.x = std::min(min.x, node.getPosition().x);
        min.y = std::min(min.y, node.getPosition().y);
        min.z = std::min(min.z, node.getPosition().z);
        max.x = std::max(max.x, node.getPosition().x);
        max.y = std::max(max.y, node.getPosition().y);
        max.z = std::max(max.z, node.getPosition().z);
    }
    auto treeSize = tree.size();
    for (unsigned i = 0; i < treeSize; ++i) {
        tree[i].getPosition() -= min;
    }
    Vector3Offload spaceSize = max - min;
    return spaceSize;
}

void SFCOctTree::calcSFCIndices(const Vector3Offload &spaceSize) {
    for (unsigned i = 0; i < tree.size(); ++i) {
        tree[i].getSFCIndex().x = uintv(std::pow(2, k)* (tree[i].getPosition().x / spaceSize.x)) - uintv(tree[i].getPosition().x == spaceSize.x);
        tree[i].getSFCIndex().y = uintv(std::pow(2, k) * (tree[i].getPosition().y / spaceSize.y)) - uintv(tree[i].getPosition().y == spaceSize.y);
        tree[i].getSFCIndex().z = uintv(std::pow(2, k) * (tree[i].getPosition().z / spaceSize.z)) - uintv(tree[i].getPosition().z == spaceSize.z);
    }
}

void SFCOctTree::sortTree() {
    std::sort(tree.begin(), tree.end());
}

void SFCOctTree::generateInternalNodes() {
    sortTree();

    auto treeSize = tree.size();
    tree.resize(treeSize * 2 - 1);

    for (unsigned i = 0; i < treeSize - 1; ++i) {
        // Count leading zeroes long long
        auto xorX = __builtin_clzll(tree[i].getSFCIndex().x ^ tree[i+1].getSFCIndex().x);
        auto xorY = __builtin_clzll(tree[i].getSFCIndex().y ^ tree[i+1].getSFCIndex().y);
        auto xorZ = __builtin_clzll(tree[i].getSFCIndex().z ^ tree[i+1].getSFCIndex().z);
        auto depth = xorX < xorY ? xorX : xorY;
        depth = depth < xorZ ? depth : xorZ;
        depth = k - unsigned(sizeof(long long) * 8 - depth);

        typedef decltype(tree[i].getSFCIndex().x) sfcType;
        sfcType mask = ~sfcType{0};
        mask >>= depth;
        mask >>= sizeof(mask) * 8 - k;

        tree[treeSize + i].getSFCIndex().x = tree[i].getSFCIndex().x | mask;
        tree[treeSize + i].getSFCIndex().y = tree[i].getSFCIndex().y | mask;
        tree[treeSize + i].getSFCIndex().z = tree[i].getSFCIndex().z | mask;
        tree[treeSize + i].getDepth() = depth;
    }
}

void SFCOctTree::removeDuplicateInternalNodes() {
    sortTree();
    tree.erase(std::unique(tree.begin(), tree.end()), tree.end());
}

void SFCOctTree::establishParentChildRel() {
//    std::cout << *this <<"\n";
    std::vector<Node> treeHelper = tree;

    for (unsigned i = 0; i < tree.size(); ++i) {
        treeHelper[i].getChildren()[7] = i;
    }
    auto treeSize = tree.size();

    treeHelper.resize(2 * treeSize - 1);

    for (unsigned i = 0; i < treeSize - 1; ++i) {
        auto shift = abs(tree[i].getDepth() - tree[i+1].getDepth()) + k - std::max(tree[i].getDepth(), tree[i+1].getDepth());
        auto xorX = __builtin_clzll((tree[i].getSFCIndex().x ^ tree[i+1].getSFCIndex().x) >> shift);
        auto xorY = __builtin_clzll((tree[i].getSFCIndex().y ^ tree[i+1].getSFCIndex().y) >> shift);
        auto xorZ = __builtin_clzll((tree[i].getSFCIndex().z ^ tree[i+1].getSFCIndex().z) >> shift);
        auto depth = xorX < xorY ? xorX : xorY;
        depth = depth < xorZ ? depth : xorZ;
        depth = k - unsigned(sizeof(long long) * 8 - depth);
        depth -= shift;

        typedef decltype(tree[i].getSFCIndex().x) sfcType;
        sfcType mask = ~sfcType{0};
        mask >>= depth;
        mask >>= sizeof(mask) * 8 - k;

        treeHelper[treeSize + i].getSFCIndex().x = tree[i].getSFCIndex().x | mask;
        treeHelper[treeSize + i].getSFCIndex().y = tree[i].getSFCIndex().y | mask;
        treeHelper[treeSize + i].getSFCIndex().z = tree[i].getSFCIndex().z | mask;
        treeHelper[treeSize + i].getDepth() = depth;
        treeHelper[treeSize + i].getChildren()[0] = i;
    }

    auto nodeCompWChildren = [](const Node &lhs, const Node &rhs){
        return lhs < rhs || lhs == rhs && lhs.getChildren()[0] < rhs.getChildren()[0];
    };

    std::sort(treeHelper.begin(), treeHelper.end(), nodeCompWChildren);

    /*std::cout << "\n\n";
    std::string tabSpace = "| ";
    for (const auto &node : treeHelper) {
        std::string tab;
        for (int i = 0; i < node.getDepth(); ++i) {
            tab += tabSpace;
        }
        std::cout << tab << node << "\n";
    }*/

    bool repeat = true;
    unsigned repeatIndex = 0;
    while (repeat) {
        for (unsigned i = 0; i < treeHelper.size() - repeatIndex - 1; ++i) {
            repeat = false;
            const auto &n1 = treeHelper[i];
            const auto &n2 = treeHelper[i + repeatIndex + 1];
            if (n1 == n2) {
                repeat = true;
                if (n1.getChildren()[0] < 0) {
//                    std::cout << n1.children.back() << " -> " << n2.children.front()<< "\n";
                    tree[n1.getChildren()[7]].getChildren()[repeatIndex] = n2.getChildren()[0];
                } /*else if (n2.getChildren()[0] < 0) {
//                    std::cout << n2.children.back() << " -> " << n1.children.front()<< "\n";
                    tree[n2.getChildren()[7]].getChildren()[repeatIndex] = n1.getChildren()[0];
                }*/
            }
        }
        repeatIndex++;
    }
}

void SFCOctTree::calculateNodeData() {
    for (unsigned i = 0; i < tree.size(); ++i) {
        if (tree[i].getDepth() != k) {
            fp mass = 0;
            Vector3Offload position = {0,0,0};
            for (unsigned j = 0; j < 8; ++j) {
                // If parallel: check tree[tree[i].getChildren()[j]].getMass() >= 0 to see if
                // child data is already calculated
                if (tree[i].getChildren()[j] < 0) {
                    break;
                }
                const auto &child = tree[tree[i].getChildren()[j]];
                mass += child.getMass();
                position += child.getPosition() * child.getMass();
            }
            position /= mass;
            tree[i].getMass() = mass;
            tree[i].getPosition() = position;
        }
    }
}

Vector3Offload SFCOctTree::calculateAcceleration(const Vector3Offload &position) const {
    Vector3Offload result = {0,0,0};

    std::vector<unsigned> stack;
    stack.reserve(k);
    stack.emplace_back(tree.size() - 1);
    while (!stack.empty()) {
        const auto &curNode = tree[stack.back()];
        stack.pop_back();

        const auto isFarEnough = [&](){
            const auto errorRate = fp(0.5);
//            return false;
            const auto diff = curNode.getPosition() - position;
//            const auto scale = std::pow(2, -curNode.getDepth());
            const auto scale = fp(1) / (1u << curNode.getDepth());
//            std::cout << "scale: " << scale << ", depth: " << curNode.getDepth() <<"\n";
            const auto x = (diff.x * treeBoundingBox.y * treeBoundingBox.z);
            const auto y = (treeBoundingBox.x * diff.y * treeBoundingBox.z);
            const auto z = (treeBoundingBox.x * treeBoundingBox.y * diff.z);
            const auto r = (treeBoundingBox.x * treeBoundingBox.y * treeBoundingBox.z) * scale / errorRate;
            const auto res = (x*x + y*y + z*z) >= (r*r);
//            const auto res2 = std::pow(((treeBoundingBox.x + treeBoundingBox.y + treeBoundingBox.z) * scale / 3), 2) / position.squareDistance(curNode.getPosition()) < errorRate * errorRate;
//            const auto rVec = treeBoundingBox * scale;
//            const auto res3 = (diff.x * diff.x) / (rVec.x * rVec.x) + (diff.y * diff.y) / (rVec.y * rVec.y) + (diff.z * diff.z) / (rVec.z * rVec.z) > fp(1)/(errorRate * errorRate);
//            const auto res4 = std::pow(((diff.x * diff.x) / ((treeBoundingBox.x * scale) * (treeBoundingBox.x * scale))
//                                        + (diff.y * diff.y) / ((treeBoundingBox.y * scale) * (treeBoundingBox.y * scale))
//                                        + (diff.z * diff.z) / ((treeBoundingBox.z * scale) * (treeBoundingBox.z * scale))), -1) < errorRate * errorRate;
//            std::cout << "tbb: " << treeBoundingBox << "\n";
//            std::cout << "res1: " << res << ", res2: " << res2 << ", res3: " << res3 << ", res4: " << res4 << "\n";
//            std::cout << "diff: " << res2 - res4 <<"\n";
//            assert(res2 - res4 < std::pow(2, -44));
//            assert(res2 == res3);
//            assert(res == res2);
            return res;
        };
//        std::cout << "node: " << curNode << std::endl;
//        std::cout << "stack size: " << stack.size() << "\n";
        if (curNode.isLeaf() && position == curNode.getPosition()) {
//            std::cout << "curNode.isLeaf() && position == curNode.getPosition()\n";
            continue;
        } else if (curNode.isLeaf() || isFarEnough()) {
//            std::cout << "curNode.isLeaf() || isFarEnough()\n";
//            if (!curNode.isLeaf()) {
//                std::cout << "is far enough: " << curNode.getPosition() << "; " << position << "\n";
//            } else {
//                std::cout << "direct contact\n";
//            }
            const Vector3Offload diff = curNode.getPosition() - position;
            const fp norm = diff.norm();
            result += diff * (curNode.getMass() / norm * norm * norm);
            continue;
        } else {
//            std::cout << "else\n";
            for (unsigned i = 0; i < 8; ++i) {
//                std::cout << i << ", " << curNode.getChildren()[i] << "\n";
                if (curNode.getChildren()[i] < 0) {
                    continue;
                }
                stack.emplace_back(curNode.getChildren()[i]);
//                std::cout << i << ", " << curNode.getChildren()[i] << "\n";
            }
        }
    }
    return result;
}

void SFCOctTree::calcNextPosition(const fp timeStep) {
    for (unsigned i = 0; i < tree.size(); ++i) {
        if (tree[i].isLeaf()) {
            auto &pos = tree[i].getPosition();
            const auto &vel = tree[i].getVelocity();
            const auto &acc = tree[i].getAcceleration();
            pos += vel * timeStep + acc * (timeStep * timeStep / 2);
        }
    }
}

std::ostream &operator<<(std::ostream &out, const SFCOctTree &tree) {
    std::string tabSpace = "| ";
    for (const auto &node : tree.tree) {
        std::string tab;
        for (int i = 0; i < node.getDepth(); ++i) {
            tab += tabSpace;
        }
        out << tab << node << "\n";
    }
    return out;
}
