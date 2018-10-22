//
// Created by thomas on 3/14/18.
//

#ifndef CHARON_BRUTEFORCEUNIVERSE_H
#define CHARON_BRUTEFORCEUNIVERSE_H

#include "BruteForceUniverseBase.h"

class BruteForceUniverse : public BruteForceUniverseBase {
public:
    BruteForceUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    void calculateFirstStep() override;

    void calculateNextStep() override;

    void calcNextPosition();
private:
    Vector3 calcAcceleration(unsigned int target) const;
};


#endif //CHARON_BRUTEFORCEUNIVERSE_H
