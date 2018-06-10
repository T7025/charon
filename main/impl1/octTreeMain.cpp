//
// Created by thomas on 4/26/18.
//

#include <iostream>
#include <cmath>
#include <base/Timer.h>
#include <iomanip>
#include <vector>
//#include "OctTreeNode.h"
using fp = float;
void expensiveOp() {

    constexpr unsigned size = 100000;
    std::vector<float> a(size);
    for (int i = 0; i < size; ++i) {
        a[i] = float(i*i)/(2*i);
    }
    auto b = a;

    auto *allocated = static_cast<int*>(omp_target_alloc(sizeof(int) * size, 0));
    for (int j = 0; j < 100; ++j) {
        #pragma omp target is_device_ptr(allocated)
        {
            for (unsigned i = 0; i < 500; ++i) {
                allocated[i] += i;
            }
        }
    }
    int host[size];
    std::cout <<"omp target is present: "<< omp_target_is_present(host, 0) <<"\n";
    omp_target_associate_ptr(host, allocated, size, 0, 0);
    std::cout <<"omp target is present: "<< omp_target_is_present(host, 0) <<"\n";

    return;


    int result0[3];
    #pragma omp target teams map(from:result0[0:3])
    {
        result0[0] = omp_get_num_devices();
        result0[1] = omp_get_num_teams();
        result0[2] = omp_get_max_threads();
    }
    std::cout << "GPU 0\n";
    std::cout << result0[0] << std::endl;
    std::cout << result0[1] << std::endl;
    std::cout << result0[2] << std::endl;
    #pragma omp target teams map(from:result0[0:3]) device(1)
    {
        result0[0] = omp_get_num_devices();
        result0[1] = omp_get_num_teams();
        result0[2] = omp_get_max_threads();
    }
    std::cout << "GPU 1\n";
    std::cout << result0[0] << std::endl;
    std::cout << result0[1] << std::endl;
    std::cout << result0[2] << std::endl;

    std::cout << omp_get_num_devices() << std::endl;
    std::cout << omp_get_num_teams() << std::endl;
    std::cout << omp_get_max_threads() << std::endl;

    //std::cout << acc_get_device_type() << std::endl;
    //std::cout << acc_get_num_devices(2) << std::endl;

    fp sum = 0.0;
//    const auto nsteps = unsigned(1.0E5);
    const auto nsteps = unsigned(2.0E5);
    std::cout << "nsteps: " << nsteps << "\n";
    std::cout << "real steps: " << fp(nsteps) * fp(nsteps) << "\n";
    fp step = fp(1.0) / (fp(nsteps) * fp(nsteps));
    fp startTime = static_cast<fp>(omp_get_wtime());
    //#pragma acc parallel loop reduction(+:sum) firstprivate(nsteps)
//    #pragma omp parallel for simd reduction(+:sum) firstprivate(nsteps)

/*    std::cout << "omp_get_num_devices: " << omp_get_num_devices() << std::endl;
    std::cout << "omp_get_num_devices: " << acc_get_num_devices(acc_device_not_host) << std::endl;*/
    //#pragma acc parallel loop reduction(+:sum)
//    #pragma omp parallel for reduction(+:sum)
//    #pragma omp target map(tofrom:sum) map(to:nsteps, step)
//    #pragma omp teams distribute parallel for simd reduction(+:sum) map(tofrom:sum) map(to:nsteps, step)
    #pragma omp target teams distribute parallel for simd map(to:nsteps, step) map(tofrom:sum) reduction(+:sum)
    for (unsigned i = 0; i < nsteps; ++i) {
        for (unsigned j = 0; j < nsteps; ++j) {
            fp x = ((fp(i) * fp(nsteps) + fp(j))+ fp(0.5)) * step;
            sum += fp(1.0) / (fp(1.0) + x * x);
        }
    }

//    std::vector<int> result(3);
//    int result[4];
    std::array<int, 4> result{};
    #pragma omp target teams map(from:result)
    {
        result[0] = (omp_is_initial_device());
        result[1] = (omp_get_team_size(0));
        result[2] = omp_get_max_threads();
        //acc_
    }
        result[3] = omp_get_num_devices();
    for (int i = 0; i < 4; ++i) {
        std::cout << result[i] << std::endl;
    }

/*
    int a = -1;
    #pragma omp target device(0) map(tofrom:sum, a)
    {
        a = omp_is_initial_device();
        #pragma omp teams distribute reduction(+:sum)

    }
    std::cout << "initial device? " << a << std::endl;
*/
    auto pi = 4.0 * step * sum;
    std::cout << std::fixed;
    std::cout << "pi is " << std::setprecision(17) << pi << "\n";
    std::cout << "time: " << omp_get_wtime() - startTime << std::endl;

    //#pragma omp target enter data


}

int main() {
    unsigned size = 500000;
    auto *allocated = static_cast<int*>(omp_target_alloc(sizeof(int) * size, 0));
    int host[size];

    std::cout <<"omp target is present: "<< omp_target_is_present(host, 0) <<"\n";
    omp_target_associate_ptr(host, allocated, size, 0, 0);
    std::cout <<"omp target is present: "<< omp_target_is_present(host, 0) <<"\n";

    for (int j = 0; j < 100; ++j) {
        #pragma omp target teams distribute parallel for is_device_ptr(allocated)
        for (int i = 0; i < 10000; ++i) {
            allocated[i] += 5;
        }
    }
    std::cout << "num devices: " << omp_get_num_devices()<<std::endl;
    std::cout << "initial device: " << omp_get_initial_device()<<std::endl;
    omp_target_memcpy(host, allocated, 7 * sizeof(int), 0, 0, omp_get_initial_device(), 0);
    for (int i = 0; i < 10; ++i) {
        std::cout << host[i] << ",";
    }
    std::cout << std::endl;

    /*Vector3 a{1,1,2};
    Vector3 b{1,1,2};
    std::cout << (a == b) << "\n";

    int max = 16;
    OctTree octTree{{0,0,0}, 16};
    for (int i = 0; i < 10; ++i) {
        octTree.addBody(1, {1.0/i, 0, 0});

    }

    octTree.printTree(std::cout);

    std::cout << octTree.calculateAcceleration({0, 0, -.5})<< std::endl;*/
    //expensiveOp();

    /*//std::vector<double> d(5000);
    Timer timer{};
    double d = 0;
    double step
    //#pragma omp parallel for reduction(+: d)
    #pragma omp target teams distribute parallel for reduction(+:d)
    for (unsigned long i = 0; i < unsigned(3E8); ++i) {
        d += expensiveOp(i, step)
    }
    std::cout << "result: " << d << "\n";
    std::cout << "time: " << timer.finish() << " sec\n";*/
}