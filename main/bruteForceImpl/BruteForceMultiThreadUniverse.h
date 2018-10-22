//
// Created by thomas on 5/8/18.
//

#ifndef CHARON_BRUTEFORCEMULTITHREADUNIVERSE_H
#define CHARON_BRUTEFORCEMULTITHREADUNIVERSE_H


#include "BruteForceUniverseBase.h"

class BruteForceMultiThreadUniverse : public BruteForceUniverseBase {
public:
    BruteForceMultiThreadUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    //~BruteForceMultiThreadUniverse() = default;

//    void addBody(fp &&mass, Vector3 &&position, Vector3 &&velocity, Vector3 &&acceleration);

//    size_t bodyCount() const;

    void calculateFirstStep() override;

    void calculateNextStep() override;

//    void logInternalState() const;

    // Used for testing
//    std::vector<Vector3> getPositions() const;

//    fp calcEnergy() const;

//    Vector3 calcMomentum() const;
    void calcNextPosition() override;
private:
//    std::vector<fp> mass;
//    std::vector<Vector3> position;
//    std::vector<Vector3> velocity;
//    std::vector<Vector3> acceleration;

    Vector3 calcAcceleration(const unsigned int target) const;
};

#endif //CHARON_BRUTEFORCEMULTITHREADUNIVERSE_H
