//
// Created by thomas on 5/23/18.
//

#ifndef CHARON_BRUTEFORCEOFFLOADUNIVERSE2_H
#define CHARON_BRUTEFORCEOFFLOADUNIVERSE2_H

#include <base/AbstractUniverse.h>
#include <base/Vector3.h>
#include <spdlog/fmt/ostr.h>
#include <omp.h>
#include <hdf5_hl.h>

#pragma omp declare target

class BruteForceOffloadUniverse2 {
public:
    BruteForceOffloadUniverse2() = default;

    void init(unsigned long size, fp timeStep, fp *mass, Vector3Offload *position,
              Vector3Offload *velocity, Vector3Offload *acceleration) {
        this->size = size;
        this->timeStep = timeStep;
        this->mass = mass;
        this->position = position;
        this->velocity = velocity;
        this->acceleration = acceleration;
    }

    void getBodyData(fp *mass, Vector3Offload *position, Vector3Offload *velocity, Vector3Offload *acceleration) {
        //memccpy(mass, this->mass, sizeof(mass), size);
    }

    void calculateFirstStep() {
        //#pragma omp target
        //#pragma omp teams distribute parallel for if(!omp_get_initial_device())
        //for (unsigned team = 0; team < omp_get_team_num(); ++team)
        #pragma omp distribute parallel for
        for (unsigned i = 0; i < size; ++i) {
            position[i] += velocity[i] * timeStep + acceleration[i] * (timeStep * timeStep);
        }
    }

    void calculateNextStep() {

    }

private:
    unsigned long size;
    fp timeStep;
    fp *mass;
    Vector3Offload *position;
    Vector3Offload *velocity;
    Vector3Offload *acceleration;

};
#pragma omp end declare target

class BruteForceOffloadUniverse2Interface : public AbstractUniverse {
public:
    BruteForceOffloadUniverse2Interface(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    ~BruteForceOffloadUniverse2Interface() override;

    void initBodies(std::shared_ptr<UniverseShape> shape) override;

    void addBody(fp mass, Vector3 position, Vector3 velocity, Vector3 acceleration) override {
        this->mass.emplace_back(mass);
        this->position.emplace_back(position);
        this->velocity.emplace_back(velocity);
        this->acceleration.emplace_back(acceleration);
    };

    size_t bodyCount() const override {
        return mass.size();
    }

    void calculateFirstStep() override {
        ++numberStepsToCalculate;
    }

    void calculateNextStep() override {
        ++numberStepsToCalculate;
    }

    void logInternalState() const override {
        calculateNSteps();
        console->info("{} bodies", mass.size());
        spdlog::drop("internalStateLogger");

        static int fileCount = 0;

        std::ostringstream fileName;
        fileName << settings->fileSettings->resultsDirPath
                 << "/" << settings->fileSettings->resultsFilenamePrefix << fileCount++ << ".csv";

        auto fileLogger = spdlog::basic_logger_mt("internalStateLogger", fileName.str());
        fileLogger->set_pattern("%v");

        for (unsigned i = 0; i < bodyCount(); ++i) {
            fileLogger->info("{},{},{},{}", mass[i], position[i], velocity[i], acceleration[i]);
        }
    }

    std::vector<Vector3> getPositions() const override {
        std::vector<Vector3> result(bodyCount());
        for (const auto &p : position) {
            result.emplace_back(p.toVector3());
        }
        return result;
    }

    fp calcEnergy() const override { return 0; };

    Vector3 calcMomentum() const override { return {0, 0, 0}; };

    void calcNextPosition() override {};
private:
    void calculateNSteps() const {
        if (numberStepsToCalculate > 0) {
            auto u = universe;
            auto nSteps = numberStepsToCalculate;
            auto isFirst = isFirstStep;
            #pragma omp target teams map(to: nSteps, isFirst)
            {
                for (unsigned step = 0; step < nSteps; ++step) {
                    if (isFirst) {
                        u->calculateFirstStep();
                        isFirst = false;
                    } else {
                        u->calculateNextStep();
                    }
                }
            }
            numberStepsToCalculate = 0;
            isFirstStep = false;
            auto size = bodyCount();
            auto m = mass.data();
            auto pos = position.data();
            auto vel = velocity.data();
            auto acc = acceleration.data();
            #pragma omp target map(from: m[0:size], pos[0:size], vel[0:size], acc[0:size])
            {
                u->getBodyData(m, pos, vel, acc);
            }
        }
    }

    mutable BruteForceOffloadUniverse2 *universe;
    mutable std::vector<fp> mass;
    mutable std::vector<Vector3Offload> position;
    mutable std::vector<Vector3Offload> velocity;
    mutable std::vector<Vector3Offload> acceleration;

    mutable unsigned numberStepsToCalculate = 0;
    mutable bool isFirstStep = true;
};


#endif //CHARON_BRUTEFORCEOFFLOADUNIVERSE2_H
