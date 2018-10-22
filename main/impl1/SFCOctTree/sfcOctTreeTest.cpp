//
// Created by thomas on 6/2/18.
//


#include <omp.h>
#include "SFCOctTree.h"

void fun1() {
    std::vector<fp> mass;
    std::vector<Vector3> pos;
    std::vector<Vector3> vel;
    std::vector<Vector3> acc;

    for (int i = 0; i < 198; ++i) {
//        mass.emplace_back(double(rand()) / RAND_MAX * 1);
        mass.emplace_back(1);
        pos.emplace_back(double(rand()) / RAND_MAX * 10, double(rand()) / RAND_MAX * 10, double(rand()) / RAND_MAX * 10);
//        pos.emplace_back(i%4, i%4, i%4);
        vel.emplace_back(double(rand()) / RAND_MAX * 10, double(rand()) / RAND_MAX * 10, double(rand()) / RAND_MAX * 10);
        acc.emplace_back(double(rand()) / RAND_MAX * 10, double(rand()) / RAND_MAX * 10, double(rand()) / RAND_MAX * 10);
    }
    pos.emplace_back(0,0,0);
    mass.emplace_back(1);
    vel.emplace_back(0,0,0);
    acc.emplace_back(0,0,0);
    pos.emplace_back(10,10,10);
    mass.emplace_back(1);
    vel.emplace_back(0,0,0);
    acc.emplace_back(0,0,0);

    SFCOctTree tree{pos.data(), vel.data(), acc.data(), mass.data(), mass.size()};

    std::cout << mass.size() << " bodies\n";
    std::cout << "Tree empty\n";
//    std::cout << tree;
    auto start = omp_get_wtime();
    tree.buildTree();
    auto stop = omp_get_wtime();
    std::cout << "Tree built in " << stop - start << " sec.\n";
//    std::cout << tree;


    start = omp_get_wtime();
//    for (const auto &position : pos) {
    #pragma omp parallel for schedule(static)
    for (unsigned i = 0; i < pos.size(); ++i) {
        const auto newAcc = tree.calculateAcceleration(pos[i]);
//        std::cout << "pos: " <<position << ", calc acc: " << newAcc << "\n";
    }
    stop = omp_get_wtime();
    std::cout << "NewAcc calc: " << stop - start << " sec.\n";

}

int main() {
    fun1();
}