//
// Created by thomas on 3/14/18.
//

#ifndef CHARON_BRUTEFORCEUNIVERSE_H
#define CHARON_BRUTEFORCEUNIVERSE_H


#include <base/AbstractUniverse.h>

class BruteForceUniverse : public AbstractUniverse {
public:
    BruteForceUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    ~BruteForceUniverse() override = default;

    void addBody(fp mass, Vector3 position, Vector3 velocity, Vector3 acceleration) override;

    size_t bodyCount() const override ;

    void calculateFirstStep() override;

    void calculateNextStep() override;

    void logInternalState() const override;

    // Used for testing
    std::vector<Vector3> getPositions() const override;

    fp calcEnergy() const override;

    Vector3 calcMomentum() const override;
    void calcNextPosition();
private:
    std::vector<fp> mass;
    std::vector<Vector3> position;
    std::vector<Vector3> velocity;
    std::vector<Vector3> acceleration;

    Vector3 calcAcceleration(const unsigned int target) const;
};


#endif //CHARON_BRUTEFORCEUNIVERSE_H
