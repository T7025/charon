//
// Created by thomas on 5/8/18.
//

#include "BruteForceOffloadUniverse.h"
#include <boost/math/special_functions/pow.hpp>
#include <spdlog/fmt/ostr.h>  // Needed to use ostream &operator<< overloads.
#include <sstream>
#include <iostream>
#include <omp.h>

BruteForceOffloadUniverse::BruteForceOffloadUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console) :
        AbstractUniverse{std::move(settings), std::move(console)} {
    /*const auto size = this->settings->universeSettings->number_bodies;//settings->universeSettings->number_bodies;
    mass.resize(size, 1);
    position.resize(size);
    for (auto &p : position) {
        p = {1,1,1};
    }
    auto *m = mass.data();
    auto *pos = position.data();
    std::cout << m << std::endl;
    #pragma omp target enter data map(to: m[0:size])
    #pragma omp target enter data map(to: pos[0:size])*/
}



BruteForceOffloadUniverse::~BruteForceOffloadUniverse() {
    auto size = bodyCount();
    auto *m = mass.data();
    auto *pos = position.data();
    auto *vel = velocity.data();
    auto *acc = acceleration.data();
    auto tStep = timeStep;
    #pragma omp target exit data map(delete: m[0:size], pos[0:size], vel[0:size], acc[0:size])
}

void BruteForceOffloadUniverse::initBodies(std::shared_ptr<UniverseShape> shape) {
    AbstractUniverse::initBodies(shape);
    initOffload();
}

void BruteForceOffloadUniverse::initOffload() {
    auto size = bodyCount();
    auto *m = mass.data();
    auto *pos = position.data();
    auto *vel = velocity.data();
    auto *acc = acceleration.data();
    auto tStep = timeStep;
    #pragma omp target enter data map(to: m[0:size], pos[0:size], vel[0:size], acc[0:size])
}

void BruteForceOffloadUniverse::addBody(fp mass, Vector3 position, Vector3 velocity, Vector3 acceleration) {
    this->mass.emplace_back(mass);
    this->position.emplace_back(position);
    this->velocity.emplace_back(velocity);
    this->acceleration.emplace_back(acceleration);
}

size_t BruteForceOffloadUniverse::bodyCount() const {
    return mass.size();
}

Vector3 BruteForceOffloadUniverse::calcAcceleration(const unsigned int target) const {
    const auto size = bodyCount();
    auto *m = mass.data();
    auto *pos = position.data();
    Vector3Offload newAcceleration{0, 0, 0};
//    #pragma omp declare reduction(+: Vector3Offload : omp_out += omp_in)
//    #pragma omp target teams distribute parallel for reduction(+:newAcceleration) map(to:size) map(tofrom:newAcceleration)
    for (unsigned j = 0; j < size; ++j) {
        if (target == j) continue;
        const auto diff = pos[j] - pos[target];
        fp norm = diff.norm();
        newAcceleration += diff * (m[j] / (norm*norm*norm));
    }
    return newAcceleration.toVector3();
}


void BruteForceOffloadUniverse::calcNextPosition() {
    auto size = bodyCount();
    auto tStep = timeStep;
    auto *pos = position.data();
    auto *vel = velocity.data();
    auto *acc = acceleration.data();
    //#pragma omp target update to(pos[0:size], vel[0:size], acc[0:size])
//    #pragma omp target map(to: size, tStep)//, vel[0:size], acc[0:size]) map(tofrom: pos[0:size])
//    #pragma omp teams distribute parallel for simd //schedule(static, 1)
//    #pragma omp target teams distribute parallel for map(to: size, tStep) //schedule(static, 1)

//    #pragma omp target teams distribute parallel for //map(tofrom: pos[0:size], vel[0:size], acc[0:size], tStep, size) //schedule(static, 1)
    int num_teams = 48;  // Grid Size Dim1, default 39 (max 39)
    int block_threads = 8;  // Block Size Dim2, default 8 (max seems 28) (If Block Size large, num teams used decreases)
    #pragma omp target
    #pragma omp teams //num_teams(num_teams) thread_limit(block_threads)
    #pragma omp distribute parallel for simd //schedule(static, 1)
    for (unsigned i = 0; i < size; ++i) {
        //pos[i] += vel[i] * tStep + acc[i] * (tStep * tStep / 2);
        pos[i].x += vel[i].x * tStep + acc[i].x * (tStep * tStep / 2);
        pos[i].y += vel[i].y * tStep + acc[i].y * (tStep * tStep / 2);
        pos[i].z += vel[i].z * tStep + acc[i].z * (tStep * tStep / 2);
        //pos[i] += (vel[i] + acc[i] * (tStep/2)) * tStep;
    }
    //#pragma omp target update from(pos[0:size])
}

/*void testNextStep(unsigned N, fp *mass, Vector3Offload *pos, Vector3Offload *vel, Vector3Offload *acc, fp timeStep) {
    for (unsigned i = 0; i < N; ++i) {
        //fp xAcc = 0, yAcc = 0, zAcc = 0;
        Vector3Offload newAcc{0,0,0};
        for (unsigned j = 0; j < N; ++j) {
            if (i == j) {
                continue;
            }
            const auto diff = pos[j] - pos[i];
            const fp norm = diff.norm();
            newAcc += diff * (mass[j] / (norm * norm * norm));
        }
        vel[i] += (acc[i] + newAcc) * (timeStep / 2);
        acc[i] = newAcc;
    }

    //return omp_target_is_present(mass, 0);
}*/

void BruteForceOffloadUniverse::calculateFirstStep() {
    const auto size = bodyCount();
    calcNextPosition();
    /*for (unsigned i = 0; i < size; ++i) {
        Vector3Offload newAcceleration{calcAcceleration(i)};
        velocity[i] += newAcceleration * timeStep;
        acceleration[i] = newAcceleration;
    }*/
    auto *m = mass.data();
    auto *pos = position.data();
    auto *vel = velocity.data();
    auto *acc = acceleration.data();

    auto tStep = timeStep;
//    #pragma omp target
    {
        A().testNextStep(size, m, pos, vel, acc, tStep);
    }

    int result = -1;
    //#pragma omp target data map(tofrom: size, m, pos, vel, acc)
    {
        //result = testNextStep((unsigned int) size, m, pos, vel, acc);
    }
    result = omp_target_is_present(m, 0);
    std::cout << __PRETTY_FUNCTION__ << " result: " <<result<<"\n";
    //for (unsigned i = 0; i < size; ++i) {
    //    std::cout << "pos: " << i << ": " << position[i].norm() << ",\t";
    //    std::cout << "vel: " << velocity[i].norm() << ",\t";
    //    std::cout << "acc: " << acceleration[i].norm() << ",\n";
    //}
    //std::cout << "momentum: " << calcMomentum() << "\n";
}


void BruteForceOffloadUniverse::calculateNextStep() {
    const auto size = bodyCount();

    calcNextPosition();
    /*for (unsigned i = 0; i < size; ++i) {
        Vector3Offload newAcceleration{calcAcceleration(i)};
        velocity[i] += (acceleration[i] + newAcceleration) * timeStep / 2;
        acceleration[i] = newAcceleration;
    }*/
    auto *m = mass.data();
    auto *pos = position.data();
    auto *vel = velocity.data();
    auto *acc = acceleration.data();

    auto tStep = timeStep;
//    #pragma omp target
    {
        A().testNextStep(size, m, pos, vel, acc, tStep);
    }

    //for (unsigned i = 0; i < size; ++i) {
    //    std::cout << "pos: " << i << ": " << position[i].norm() << ",\t";
    //    std::cout << "vel: " << velocity[i].norm() << ",\t";
    //    std::cout << "acc: " << acceleration[i].norm() << ",\n";
    //}
    //std::cout << "momentum: " << calcMomentum() << "\n";
}

void BruteForceOffloadUniverse::logInternalState() const {
    console->info("{} bodies", mass.size());
    spdlog::drop("internalStateLogger");

    static int fileCount = 0;

    std::ostringstream fileName;
    fileName << settings->fileSettings->resultsDirPath
             << "/" << settings->fileSettings->resultsFilenamePrefix << fileCount++ << ".csv";

    auto fileLogger = spdlog::basic_logger_mt("internalStateLogger", fileName.str());
    fileLogger->set_pattern("%v");

    const auto size = bodyCount();
    //auto *m = mass.data();
    auto *pos = position.data();
    auto *vel = velocity.data();
    auto *acc = acceleration.data();
    #pragma omp target update from(pos[0:size], vel[0:size], acc[0:size])
    fileLogger->info("mass,xPos,yPos,zPos,xVel,yVel,zVel,xAcc,yAcc,zAcc");
    for (unsigned i = 0; i < bodyCount(); ++i) {
        fileLogger->info("{},{},{},{}", mass[i], position[i], velocity[i], acceleration[i]);
    }
}

std::vector<Vector3> BruteForceOffloadUniverse::getPositions() const {
    const auto size = bodyCount();
    auto *pos = position.data();
    #pragma omp target update from(pos[0:size])

    std::vector<Vector3> result;
    result.reserve(bodyCount());
    for (const auto &pos : position) {
        result.push_back(pos.toVector3());
    }
    return result;
}

fp BruteForceOffloadUniverse::calcEnergy() const {
    fp result = 0;
    for (unsigned i = 0; i < bodyCount(); ++i) {
        auto velNorm = velocity[i].norm();
        result += mass[i] * velNorm * velNorm / 2;
    }
    return result;
}

Vector3 BruteForceOffloadUniverse::calcMomentum() const {
    const auto size = bodyCount();
    Vector3Offload result = {0,0,0};
    for (unsigned i = 0; i < size; ++i) {
        result += velocity[i] * mass[i];
    }
    return result.toVector3();
}
