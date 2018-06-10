//
// Created by thomas on 6/1/18.
//

#include <bitset>
#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>
#include <base/Vector3.cpp>

constexpr unsigned k = 64;

using uintv = uint64_t;

/// Checks if the most significant bit of x is less than the most significant bit of y
bool less_msb(uintv x, uintv y) {
    return x < y && x < (x ^ y);
}

/// From wikipedia: Z-order curve
struct SFCIndex {
    uintv x, y, z;

    bool operator==(const SFCIndex &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator<(const SFCIndex &other) const {
        unsigned j = 0;
        uintv t1 = 0;
        uintv t2 = x ^ other.x;
        if (less_msb(t1, t2)) {
            j = 0;
            t1 = t2;
        }
        t2 = y ^ other.y;
        if (less_msb(t1, t2)) {
            j = 1;
            t1 = t2;
        }
        t2 = z ^ other.z;
        if (less_msb(t1, t2)) {
            j = 2;
            t1 = t2;
        }
        bool result;
        switch (j) {
            case 0:
                result = x < other.x;
                break;
            case 1:
                result = y < other.y;
                break;
            case 2:
                result = z < other.z;
                break;
            default:break;
        }
        return result;
    }
};

std::ostream &operator<<(std::ostream &out, const SFCIndex &sfcIndex) {
    out << std::bitset<k>{sfcIndex.x} << "," << std::bitset<k>{sfcIndex.y} << "," << std::bitset<k>{sfcIndex.z};
    return out;
}

struct Node {
    Node(const Vector3 &pos, const unsigned depth) : position{pos}, depth{depth} {children.fill(-1);};
    Node() = default;

    SFCIndex sfcIndex;
    std::bitset<3*k> sfcIndex2;
    Vector3 position;
    unsigned depth;
    std::array<int, 8> children;

    bool operator<(const Node &other) const {
//        return depth > other.depth || (depth == other.depth && sfcIndex < other.sfcIndex);
        return sfcIndex < other.sfcIndex || other.sfcIndex == sfcIndex && depth > other.depth;
    }
};

std::ostream &operator<<(std::ostream &out, const Node &node) {
    out << node.sfcIndex << "; "
        //<< node.sfcIndex2 << "; "
        << node.position <<"; " <<node.depth<<"; ";
    for (const auto &child : node.children) {
        out <<child<<",";
    }
    return out;
}

template <typename T>
void sort(T *arr, const size_t begin, const size_t end) {

}

template<std::size_t N>
bool operator<(const std::bitset<N>& x, const std::bitset<N>& y)
{
    for (int i = N-1; i >= 0; i--) {
        if (x[i] ^ y[i]) return y[i];
    }
    return false;
}

void test() {
    std::vector<Vector3> points = {{2,2,2}, {2,2,1}, {2,1,2}, {1,2,2}, {1,1,2}, {1,2,1}, {2,1,1}, {1,1,1}, {0,0,0}, {4,1,3}, {9.999999, 5, 1}, {9.99999900000001, 5, 1}};
//    points.push_back({2,2,2});
    for (int i = 0; i < 200; ++i) {
        points.emplace_back(double(rand()) / RAND_MAX * 10, double(rand()) / RAND_MAX * 10, double(rand()) / RAND_MAX * 10);
    }

    Vector3 min = {
            std::numeric_limits<fp>::infinity(),
            std::numeric_limits<fp>::infinity(),
            std::numeric_limits<fp>::infinity()
    };
    Vector3 max = {
            -std::numeric_limits<fp>::infinity(),
            -std::numeric_limits<fp>::infinity(),
            -std::numeric_limits<fp>::infinity()
    };
    for (const auto &point : points) {
        min.x = std::min(min.x, point.x);
        min.y = std::min(min.y, point.y);
        min.z = std::min(min.z, point.z);
        max.x = std::max(max.x, point.x);
        max.y = std::max(max.y, point.y);
        max.z = std::max(max.z, point.z);
    }
    std::cout << "min: " << min << ", max: " << max << "\n";

    std::vector<Node> tree;

    auto printTree = [&](std::string tabSpace = "|  ") {
        std::cout << "Begin tree:\n";
        for (const auto &node : tree) {
            std::string tab;
            for (int i = 0; i < node.depth; ++i) {
                tab += tabSpace;
            }
            std::cout << tab << node << "\n";
        }
    };

    for (const auto &point : points) {
        tree.emplace_back(point, 0);
    }
    for (auto &point : points) {
        point -= min;
    }
    Vector3 spaceSize = max - min;
    std::cout << "spaceSize (D): " << spaceSize << "\n";

    for (auto &node : tree) {
        uintv coordinateX = static_cast<uintv>(std::pow(2, k) * (node.position.x / spaceSize.x)) - static_cast<uintv>(node.position.x == spaceSize.x);
        uintv coordinateY = static_cast<uintv>(std::pow(2, k) * (node.position.y / spaceSize.y)) - static_cast<uintv>(node.position.y == spaceSize.y);
        uintv coordinateZ = static_cast<uintv>(std::pow(2, k) * (node.position.z / spaceSize.z)) - static_cast<uintv>(node.position.z == spaceSize.z);
//        std::cout << std::bitset<k>{coordinateX} << "\n";
//        std::cout << std::bitset<k>{coordinateY} << "\n";
//        std::cout << std::bitset<k>{coordinateZ} << "\n";

        std::bitset<3*k> sfcIndex = 0;
        for (unsigned i = 0; i < k; ++i) {
            std::bitset<3*k> t;
            t[0] = true;
            t << i;
            sfcIndex |= (coordinateX & (1u << i)) << (2 + i * 2);
            sfcIndex |= (coordinateY & (1u << i)) << (1 + i * 2);
            sfcIndex |= (coordinateZ & (1u << i)) << (0 + i * 2);
        }
        node.sfcIndex2 = sfcIndex;
//        std::cout << sfcIndex << "\n";
        node.sfcIndex.x = static_cast<uintv>(std::pow(2, k) * (node.position.x / spaceSize.x)) - static_cast<uintv>(node.position.x == spaceSize.x);
        node.sfcIndex.y = static_cast<uintv>(std::pow(2, k) * (node.position.y / spaceSize.y)) - static_cast<uintv>(node.position.y == spaceSize.y);
        node.sfcIndex.z = static_cast<uintv>(std::pow(2, k) * (node.position.z / spaceSize.z)) - static_cast<uintv>(node.position.z == spaceSize.z);

//        node.sfcIndex.x = reinterpret_cast<uintv&>(node.position.x);
//        node.sfcIndex.y = reinterpret_cast<uintv&>(node.position.y);
//        node.sfcIndex.z = reinterpret_cast<uintv&>(node.position.z);
        if (node.position.x == spaceSize.x) {
            std::cout << "x: "<< node.position.x << "\n";
        }
        if (node.position.y == spaceSize.y) {
            std::cout << "y: "<< node.position.y << "\n";
        }
        if (node.position.z == spaceSize.z) {
            std::cout << "z: "<< node.position.z << "\n";
        }
        node.depth = k;
//        node.size *= spaceSize;


        std::cout << node << "\n";
    }
//    for (const auto &node1 : tree) {
//        for (const auto &node2 : tree) {
//            if ((node1.sfcIndex < node2.sfcIndex) != (node1.sfcIndex2 < node2.sfcIndex2)) {
//                std::cout << "QOISDJFQOSDIPJ\n";
//                std::cout << node1 << "\t|\t "<< (node1.sfcIndex < node2.sfcIndex) << "\n";
//                std::cout << node2 << "\t|\t "<< (node1.sfcIndex2 < node2.sfcIndex2) << "\n";
//
//            }
//        }
//    }

    std::sort(tree.begin(), tree.end());
//    printTree();

    for (unsigned i = 0; i < tree.size() - 1; ++i) {
        if (tree[i].position == tree[i+1].position) {
            std::cout << "SQMLDKFJFIDSOFN-------------------\n";
        }
    }

    auto treeSize = tree.size();
    for (unsigned i = 1; i < treeSize; ++i) {
        // Count leading zeroes long long
        auto xorX = __builtin_clzll(tree[i-1].sfcIndex.x ^ tree[i].sfcIndex.x);
        auto xorY = __builtin_clzll(tree[i-1].sfcIndex.y ^ tree[i].sfcIndex.y);
        auto xorZ = __builtin_clzll(tree[i-1].sfcIndex.z ^ tree[i].sfcIndex.z);
        auto depth = xorX < xorY ? xorX : xorY;
        depth = depth < xorZ ? depth : xorZ;
        depth = k - unsigned(sizeof(long long) * 8 - depth);
//        std::cout << depth;  // count left zero
//        std::cout <<"\n";
        tree.emplace_back(Vector3{-1,-1,-1}, depth);
        typedef decltype(tree[i].sfcIndex.x) sfcType;
        sfcType mask = ~sfcType{0};
        mask >>= depth;
        mask >>= sizeof(mask) * 8 - k;
        std::cout << "mask: " << std::bitset<sizeof(mask) * 8>{mask} <<"\n";
        tree.back().sfcIndex.x = tree[i].sfcIndex.x | mask;
        tree.back().sfcIndex.y = tree[i].sfcIndex.y | mask;
        tree.back().sfcIndex.z = tree[i].sfcIndex.z | mask;
        std::bitset<k*3> mask2;
        mask2.flip();
        mask2 >>= depth * 3;
        tree.back().sfcIndex2 = tree[i].sfcIndex2 | mask2;
//        tree.back().sfcIndex.x = tree[i].sfcIndex.x >> depth;
//        tree.back().sfcIndex.y = tree[i].sfcIndex.y >> depth;
//        tree.back().sfcIndex.z = tree[i].sfcIndex.z >> depth;
//        tree.back().sfcIndex2 = tree[i].sfcIndex2 >> (depth*3);
    }
    std::sort(tree.begin() + treeSize, tree.end());
//    printTree("");

    for (auto i = tree.size() - 1; i-- > treeSize; ) {
        if ((tree[i + 1].sfcIndex == tree[i].sfcIndex) && (tree[i + 1].depth == tree[i].depth)) {
//            std::cout << "remove: " << (*(tree.begin() + i + 1)) << "\n";
//            std::cout << "becaus: " << (*(tree.begin() + i)) << "\n";
            tree.erase(tree.begin() + i + 1);
        }
    }

//    printTree("");
    std::sort(tree.begin(), tree.end());
//    printTree();

    std::vector<Node> treeHelper = tree;
    for (unsigned i = 0; i < treeHelper.size(); ++i) {
        treeHelper[i].children.back() = i;
    }

    treeSize = tree.size();
    for (unsigned i = 0; i < treeSize - 1; ++i) {
        // Get Least Common Ancestor
//        auto shift = abs(tree[i].depth - tree[i+1].depth);
        auto shift = abs(tree[i].depth - tree[i+1].depth) + k - std::max(tree[i].depth, tree[i+1].depth);
        auto xorX = __builtin_clzll((tree[i].sfcIndex.x ^ tree[i+1].sfcIndex.x) >> shift);
        auto xorY = __builtin_clzll((tree[i].sfcIndex.y ^ tree[i+1].sfcIndex.y) >> shift);
        auto xorZ = __builtin_clzll((tree[i].sfcIndex.z ^ tree[i+1].sfcIndex.z) >> shift);
        auto depth = xorX < xorY ? xorX : xorY;
        depth = depth < xorZ ? depth : xorZ;
        depth = k - unsigned(sizeof(long long) * 8 - depth);
        depth -= shift;
//        std::cout <<"depth: " << depth << "\n";
//        std::cout <<"shift: " << shift << "\n";
        //depth = depth - (k - std::min(tree[i].depth, tree[i+1].depth));
//        continue;
        treeHelper.emplace_back(Vector3{-1,-1,-1}, depth);
        typedef decltype(tree[i].sfcIndex.x) sfcType;
        sfcType mask = ~sfcType{0};
        mask >>= depth;
        mask >>= sizeof(mask) * 8 - k;
//        std::cout << "mask: " << std::bitset<sizeof(mask) * 8>{mask} <<"\n";
        treeHelper.back().sfcIndex.x = tree[i].sfcIndex.x | mask;
        treeHelper.back().sfcIndex.y = tree[i].sfcIndex.y | mask;
        treeHelper.back().sfcIndex.z = tree[i].sfcIndex.z | mask;
        std::bitset<k*3> mask2;
        mask2.flip();
        mask2 >>= depth * 3;
        treeHelper.back().sfcIndex2 = tree[i].sfcIndex2 | mask2;

//        treeHelper.back().children.back() = tree[i].children.back();
        treeHelper.back().children.front() = i;

    }

    auto printTreeHelper = [&](std::string tabSpace = "|  ") {
        std::cout << "Begin tree:\n";
        for (const auto &node : treeHelper) {
            std::string tab;
            for (int i = 0; i < node.depth; ++i) {
                tab += tabSpace;
            }
            std::cout << tab << node << "\n";
        }
    };

//    printTreeHelper();
    sort(treeHelper.begin(), treeHelper.end());
    printTreeHelper();

    bool repeat = true;
    unsigned repeatIndex = 0;
    while (repeat) {
        for (unsigned i = 0; i < treeHelper.size() - repeatIndex - 1; ++i) {
            repeat = false;
            const auto &n1 = treeHelper[i];
            const auto &n2 = treeHelper[i + repeatIndex + 1];
            if (n1.sfcIndex == n2.sfcIndex && n1.depth == n2.depth) {
                repeat = true;
                if (n1.children[0] < 0) {
//                    std::cout << n1.children.back() << " -> " << n2.children.front()<< "\n";
                    tree[n1.children.back()].children[repeatIndex] = n2.children.front();
                } else if (n2.children[0] < 0) {
//                    std::cout << n2.children.back() << " -> " << n1.children.front()<< "\n";
                    tree[n2.children.back()].children[repeatIndex] = n1.children.front();
                }
            }
        }
        repeatIndex++;
    }
//    printTree();
}

int main() {
    test();
}