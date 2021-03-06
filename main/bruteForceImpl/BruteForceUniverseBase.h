//
// Created by thomas on 6/13/18.
//

#ifndef CHARON_BRUTEFORCEUNIVERSEBASE_H
#define CHARON_BRUTEFORCEUNIVERSEBASE_H

#include <base/AbstractUniverse.h>

class BruteForceUniverseBase : public AbstractUniverse {
public:
    BruteForceUniverseBase(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console)
            : AbstractUniverse(std::move(settings), std::move(console)) {
    }

    void addBody(fp &&mass, Vector3 &&position, Vector3 &&velocity, Vector3 &&acceleration) override {
        this->mass.push_back(mass);
        this->position.push_back(position);
        this->velocity.push_back(velocity);
        this->acceleration.push_back(acceleration);
    }

    inline size_t bodyCount() const override {
        return mass.size();
    }

    void logInternalState() const override {
        console->info("{} bodies", mass.size());
        spdlog::drop("internalStateLogger");

        static int fileCount = 0;

        std::ostringstream fileName;
        fileName << settings->fileSettings->resultsDirPath
                 << "/" << settings->fileSettings->resultsFilenamePrefix << fileCount++ << ".csv";

        auto fileLogger = spdlog::basic_logger_mt("internalStateLogger", fileName.str());
        fileLogger->set_pattern("%v");

        fileLogger->info("mass,xPos,yPos,zPos,xVel,yVel,zVel,xAcc,yAcc,zAcc");
        for (unsigned i = 0; i < bodyCount(); ++i) {
            fileLogger->info("{},{},{},{}", mass[i], position[i], velocity[i], acceleration[i]);
        }
    }

    std::vector<Vector3> getPositions() const override {
        return position;
    }

    fp calcEnergy() const override {
        fp result = 0;
        for (unsigned i = 0; i < bodyCount(); ++i) {
            auto velNorm = velocity[i].norm();
            result += mass[i] * velNorm * velNorm / 2;
        }
        return result;
    }

    Vector3 calcMomentum() const override {
        const auto size = bodyCount();
        Vector3 result = {0,0,0};
        for (unsigned i = 0; i < size; ++i) {
            result += velocity[i] * mass[i];
        }
        return result;
    }

protected:
    std::vector<fp> mass;
    std::vector<Vector3> position;
    std::vector<Vector3> velocity;
    std::vector<Vector3> acceleration;
};


#endif //CHARON_BRUTEFORCEUNIVERSEBASE_H
