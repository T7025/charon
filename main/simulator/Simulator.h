//
// Created by thomas on 3/13/18.
//

#ifndef CHARON_SIMULATOR_H
#define CHARON_SIMULATOR_H


#include <memory>
#include <base/AbstractUniverse.h>
#include <settings/Settings.h>

class Simulator {
public:
    Simulator(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    template <typename UniverseType, typename UniverseShape>
    void setup();
    void setup2();

    //template <typename UniverseType, typename UniverseShape>
    //void setup(const UniverseSettings &universeSettings, std::shared_ptr<spdlog::logger> console);

    /**
     * RunConfig:
     * - nrOfSteps
     * - initalState
     * - (opt.) enableCollisions
     */
    void run();

    void snapShot() const;

private:
    const unsigned nrOfSteps;
    const unsigned snapshotDelta;
    std::shared_ptr<AbstractUniverse> universe;
    std::shared_ptr<Settings> settings;
    std::shared_ptr<spdlog::logger> console;
};

template <typename UniverseType, typename UniverseShape>
void Simulator::setup() {
    universe = std::make_shared<UniverseType>(settings, console);
    UniverseShape{universe}();
}
/*
template <typename UniverseType, typename UniverseShape>
void Simulator::setup(const UniverseSettings &universeSettings, std::shared_ptr<spdlog::logger> console) {
    universe = std::make_shared<UniverseType>(universeSettings, console);
    UniverseShape{universe}();
}*/

#endif //CHARON_SIMULATOR_H
