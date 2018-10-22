//
// Created by thomas on 5/23/18.
//

#include "BruteForceOffloadUniverse2.h"

BruteForceOffloadUniverse2Interface::BruteForceOffloadUniverse2Interface(std::shared_ptr<Settings> settings,
                                                                         std::shared_ptr<spdlog::logger> console)
        : AbstractUniverse(settings, console), universe{new BruteForceOffloadUniverse2{}} {
    auto u = universe;
    #pragma omp target enter data map(to: u)
}

BruteForceOffloadUniverse2Interface::~BruteForceOffloadUniverse2Interface() {
    auto u = universe;
    #pragma omp target exit data map(delete: u)
    delete(u);
}

void BruteForceOffloadUniverse2Interface::initBodies(std::shared_ptr<UniverseShape> shape)  {
    AbstractUniverse::initBodies(shape);
    auto u = universe;
    auto size = bodyCount();
    auto ts = timeStep;
    auto m = mass.data();
    auto pos = position.data();
    auto vel = velocity.data();
    auto acc = acceleration.data();
    #pragma omp target map(tofrom: u) map(to: size, ts, m[0:size], pos[0:size], vel[0:size], acc[0:size])
    {
        u->init(size, ts, m, pos, vel, acc);
    }
}

/*#pragma omp declare target
BruteForceOffloadUniverse2::BruteForceOffloadUniverse2(unsigned long size, fp timeStep, fp *mass, Vector3Offload *position,
                                                       Vector3Offload *velocity, Vector3Offload *acceleration) {

}
#pragma omp end declare target*/

