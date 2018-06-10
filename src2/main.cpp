


#include "Simulator.h"
#include "Vector3.h"
#include <fstream>
#include <array>

struct StructVector {
    double x, y, z;
};

int main() {
    std::fstream out("out.csv", std::fstream::out);
    std::cout << sizeof(std::array<double, 10>) << std::endl;
    std::vector<double> vec(10, 0);
    std::cout << sizeof(vec) << std::endl;

    double dt = 0.1;
    double v = 10.0;
    double a = 200.0;
    std::cout << v*dt + a*dt*dt/2 << std::endl;
    std::cout << (v + a*dt/2) * dt << std::endl;
//    Simulator simulator{out};
//    simulator.run();
    return 0;
}