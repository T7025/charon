//
// Created by thomas on 6/16/18.
//

#include <base/intel_stable_sort/parallel_stable_sort.h>
#include "BarnesHutMultiThreadUniverse.h"

BarnesHutMultiThreadUniverse::BarnesHutMultiThreadUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console)
        : BarnesHutUniverseBase(std::move(settings), std::move(console)) {

}

void BarnesHutMultiThreadUniverse::calculateFirstStep() {
    calcNextPosition();
    buildTree();

    #pragma omp parallel for
    for (unsigned i = 0; i < tree.size(); ++i) {
        if (tree[i].isLeaf()) {
            Vector3 newAcceleration = calculateAcceleration(tree[i]);
            tree[i].getVelocity() += newAcceleration * timeStep;
            tree[i].getAcceleration() = newAcceleration;
        }
    }

    flattenTree();
    calcNextPosition();
}

void BarnesHutMultiThreadUniverse::calculateNextStep() {
    buildTree();

    #pragma omp parallel for
    for (unsigned i = 0; i < tree.size(); ++i) {
        if (tree[i].isLeaf()) {
            Vector3 newAcceleration = calculateAcceleration(tree[i]);
            tree[i].getVelocity() += (tree[i].getAcceleration() + newAcceleration) * timeStep / 2;
            tree[i].getAcceleration() = newAcceleration;
        }
    }

    flattenTree();
    calcNextPosition();
}

void BarnesHutMultiThreadUniverse::calcNextPosition() {
    assert(!treeIsBuilt);

    const auto size = bodyCount();
    #pragma omp parallel for
    for (unsigned i = 0; i < size; ++i) {
        auto &pos = tree[i].getPosition();
        const auto &vel = tree[i].getVelocity();
        const auto &acc = tree[i].getAcceleration();
        pos += vel * timeStep + acc * (timeStep * timeStep / 2);
    }
}

Vector3 BarnesHutMultiThreadUniverse::calculateAcceleration(const Node &targetNode) const {
    Vector3 result = {0,0,0};

    std::vector<unsigned> stack;
    stack.reserve(k);
    stack.emplace_back(tree.size() - 1);
    while (!stack.empty()) {
        const auto &curNode = tree[stack.back()];
        stack.pop_back();

        const auto isFarEnough = [&](){
            if (errorRate == 0) {
                return false;
            }
            const auto diff = curNode.getPosition() - targetNode.getPosition();
            const auto scale = fp(1) / (uintv(1) << curNode.getDepth());
            const auto x = (diff.x * treeBoundingBox.y * treeBoundingBox.z);
            const auto y = (treeBoundingBox.x * diff.y * treeBoundingBox.z);
            const auto z = (treeBoundingBox.x * treeBoundingBox.y * diff.z);
            const auto r = (treeBoundingBox.x * treeBoundingBox.y * treeBoundingBox.z) * scale / errorRate;
            const auto res = (x*x + y*y + z*z) >= (r*r);
            return res;
        };
        if (curNode.isLeaf() && targetNode.getPosition() == curNode.getPosition()) {
            continue;
        } else if ((curNode.isLeaf() || isFarEnough()) && !curNode.isParentOf(targetNode)) {
            const Vector3 diff = curNode.getPosition() - targetNode.getPosition();
            const fp norm = diff.norm();
            result += diff * (curNode.getMass() / (norm * norm * norm));
            continue;
        } else {
            for (unsigned i = 0; i < 8; ++i) {
                if (curNode.getChildren()[i] < 0) {
                    continue;
                }
                stack.emplace_back(curNode.getChildren()[i]);
            }
        }
    }
    return result;
}

void BarnesHutMultiThreadUniverse::flattenTree() {
    assert(treeIsBuilt);
//    std::sort(tree.begin(), tree.end(), [](const Node &lhs, const Node &rhs){ return lhs.getDepth() > rhs.getDepth(); });
    pss::parallel_stable_sort(tree.begin(), tree.end(),
                              [](const Node &lhs, const Node &rhs){ return lhs.getDepth() > rhs.getDepth(); });
//    sortTree();
    tree.resize(bodyCount());
    treeIsBuilt = false;
}

void BarnesHutMultiThreadUniverse::buildTree() {
    scalePositions();
//    std::cout << "Scaled positions" << std::endl;
    calcSFCIndices();
//    std::cout << "Calculated SFCIndices" << std::endl;
    #pragma omp parallel for
    for (unsigned i = 0; i < tree.size() - 1; ++i) {
        if ((tree[i] == tree[i+1])) {
            std::cout << tree[i] << "\n" << tree[i+1] << "\n";
            assert(!(tree[i] == tree[i+1]));
        }
    }
    generateInternalNodes();
//    std::cout << "Generated internal nodes" << std::endl;
    removeDuplicateInternalNodes();
//    std::cout << "Removed duplicate internal nodes" << std::endl;
    establishParentChildRel();
//    std::cout << "Established parent-child relation" << std::endl;
    calculateNodeData();
//    std::cout << "Calculated node data" << std::endl;
    treeIsBuilt = true;
}

void BarnesHutMultiThreadUniverse::sortTree() {
//    std::sort(tree.begin(), tree.end());
    pss::parallel_stable_sort(tree.begin(), tree.end(), [](const Node &lhs, const Node &rhs){ return lhs < rhs;} );
}

void BarnesHutMultiThreadUniverse::scalePositions() {

    Vector3 vMin = {
            std::numeric_limits<fp>::infinity(),
            std::numeric_limits<fp>::infinity(),
            std::numeric_limits<fp>::infinity()
    };
    Vector3 vMax = {
            -std::numeric_limits<fp>::infinity(),
            -std::numeric_limits<fp>::infinity(),
            -std::numeric_limits<fp>::infinity()
    };
    const auto size = tree.size();
    #pragma omp declare reduction(min : Vector3 : omp_out.min(omp_in)) \
        initializer(omp_priv = {std::numeric_limits<fp>::infinity(), std::numeric_limits<fp>::infinity(), std::numeric_limits<fp>::infinity()})
    #pragma omp declare reduction(max : Vector3 : omp_out.max(omp_in)) \
        initializer(omp_priv = {-std::numeric_limits<fp>::infinity(), -std::numeric_limits<fp>::infinity(), -std::numeric_limits<fp>::infinity()})
    #pragma omp parallel for reduction(min : vMin) reduction(max : vMax)
    for (unsigned i = 0; i < size; ++i) {
        vMin.min(tree[i].getPosition());
        vMax.max(tree[i].getPosition());
    }
    offset = vMin;
    treeBoundingBox = vMax - vMin;
    treeBoundingBox.x = std::max(treeBoundingBox.x, std::numeric_limits<fp>::min());
    treeBoundingBox.y = std::max(treeBoundingBox.y, std::numeric_limits<fp>::min());
    treeBoundingBox.z = std::max(treeBoundingBox.z, std::numeric_limits<fp>::min());
}

void BarnesHutMultiThreadUniverse::calcSFCIndices() {
    const fp mult = std::pow(2, k);
    #pragma omp parallel for
    for (unsigned i = 0; i < tree.size(); ++i) {
        const auto pos = tree[i].getPosition() - offset;
        tree[i].getSFCIndex().x = uintv(mult * (pos.x / treeBoundingBox.x)) - uintv(pos.x == treeBoundingBox.x);
        tree[i].getSFCIndex().y = uintv(mult * (pos.y / treeBoundingBox.y)) - uintv(pos.y == treeBoundingBox.y);
        tree[i].getSFCIndex().z = uintv(mult * (pos.z / treeBoundingBox.z)) - uintv(pos.z == treeBoundingBox.z);
    }
}

void BarnesHutMultiThreadUniverse::generateInternalNodes() {
    sortTree();

    auto treeSize = tree.size();
    tree.resize(2 * treeSize - 1);

    #pragma omp parallel for
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
        mask >>= depth;  // depth < k <= sizeof(uintv) * 8 -> OK, no need of rshift()
        mask >>= sizeof(mask) * 8 - k;  // sizeof(mask) * 8 - k < sizeof(mask) * 8 -> OK

        tree[treeSize + i].getSFCIndex().x = tree[i].getSFCIndex().x | mask;
        tree[treeSize + i].getSFCIndex().y = tree[i].getSFCIndex().y | mask;
        tree[treeSize + i].getSFCIndex().z = tree[i].getSFCIndex().z | mask;
        tree[treeSize + i].getDepth() = depth;
    }
}

void BarnesHutMultiThreadUniverse::removeDuplicateInternalNodes() {
    sortTree();
    tree.erase(std::unique(tree.begin(), tree.end()), tree.end());
}

void BarnesHutMultiThreadUniverse::establishParentChildRel() {
    auto treeSize = tree.size();

    tree.resize(2 * treeSize - 1);

    #pragma omp parallel for
    for (unsigned i = 0; i < treeSize - 1; ++i) {
        auto shift = k - std::min(tree[i].getDepth(), tree[i+1].getDepth());
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

        tree[treeSize + i].getSFCIndex().x = tree[i].getSFCIndex().x | mask;
        tree[treeSize + i].getSFCIndex().y = tree[i].getSFCIndex().y | mask;
        tree[treeSize + i].getSFCIndex().z = tree[i].getSFCIndex().z | mask;
        tree[treeSize + i].getDepth() = depth;
        tree[treeSize + i].getChildren()[0] = i;
    }

    auto nodeCompWChildren = [](const Node &lhs, const Node &rhs){
        return lhs < rhs || (lhs == rhs && lhs.getChildren()[0] < rhs.getChildren()[0]);
    };

    pss::parallel_stable_sort(tree.begin() + treeSize, tree.end(), nodeCompWChildren);
    #pragma omp parallel for
    for (unsigned i = treeSize; i < tree.size(); ++i) {
        if (i == tree.size() - 1 || !(tree[i] == tree[i+1])) {
            unsigned j;
            for (j = 1; j < 8; ++j) {
                if (tree[i] == tree[i - j] && i - j >= treeSize) {
                    tree[i].getChildren()[j] = tree[i - j].getChildren()[0];
                } else {
                    break;
                }
            }
            const auto parentIndex = tree[i].getChildren()[0] + 1;
            // 0 < j < 8
            for (unsigned k = 0; k < j; ++k) {
                tree[parentIndex].getChildren()[k] = tree[i].getChildren()[j - 1 - k];
            }
        }
    }
//    std::cout << *this <<"\n";
    tree.resize(treeSize);
//    std::cout << *this <<"\n";
}

void BarnesHutMultiThreadUniverse::calculateNodeData() {
    //int ctr = 0;

    #pragma omp parallel for schedule(static) //reduction(+ : ctr)
    for (unsigned i = 0; i < tree.size(); ++i) {
        if (tree[i].getDepth() != k) {
            fp mass = 0;
            Vector3 position = {0,0,0};
            for (unsigned j = 0; j < 8; ++j) {
                // If parallel: check tree[tree[i].getChildren()[j]].getMass() >= 0 to see if
                // child data is already calculated
                if (tree[i].getChildren()[j] < 0) {
                    break;
                }
                const auto &child = tree[tree[i].getChildren()[j]];
                while (child.getMass() < 0) {
                    //++ctr;
                }
                assert(child.getMass() >= 0);
                mass += child.getMass();
                position += child.getPosition() * child.getMass();
            }
            position /= mass;
            tree[i].getMass() = mass;
            tree[i].getPosition() = position;
        }
    }
    //std::cout << "ctr: " <<ctr<<"\n";
}