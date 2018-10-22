//
// Created by thomas on 5/8/18.
//

#ifndef CHARON_BRUTEFORCEOFFLOADUNIVERSE_H
#define CHARON_BRUTEFORCEOFFLOADUNIVERSE_H


#include <base/AbstractUniverse.h>
#include <base/Vector3.h>
#include <omp.h>

class BruteForceOffloadUniverse : public AbstractUniverse {
public:
    BruteForceOffloadUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    ~BruteForceOffloadUniverse() override;

    void initBodies(std::shared_ptr<UniverseShape> shape) override;

    void initOffload();

    void addBody(fp mass, Vector3 position, Vector3 velocity, Vector3 acceleration) override;

    size_t bodyCount() const override;

    void calcNextPosition() override;


//    std::vector<Vector3> position;

    void calculateFirstStep() override;

    void calculateNextStep() override;

    void logInternalState() const override;

    // Used for testing
    std::vector<Vector3> getPositions() const override;

    fp calcEnergy() const override;

    Vector3 calcMomentum() const override;
private:
    std::vector<fp> mass;
    std::vector<Vector3Offload> position;
    std::vector<Vector3Offload> velocity;
    std::vector<Vector3Offload> acceleration;

    Vector3 calcAcceleration(const unsigned int target) const;
};

//#pragma omp declare target
class A {
public:
    void
    testNextStep(unsigned N, fp *mass, Vector3Offload *pos, Vector3Offload *vel, Vector3Offload *acc, fp timeStep) {
        #pragma omp target teams // distribute parallel for
        for (unsigned i = 0; i < N; ++i) {
            //fp xAcc = 0, yAcc = 0, zAcc = 0;
            Vector3Offload newAcc{0, 0, 0};
            //#pragma omp distribu
            #pragma omp distribute parallel for
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
    }

};
//#pragma omp end declare target

#endif //CHARON_BRUTEFORCEOFFLOADUNIVERSE_H
