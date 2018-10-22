//
// Created by thomas on 3/13/18.
//

#include "AbstractUniverse.h"
#include <utility>

AbstractUniverse::AbstractUniverse(std::shared_ptr<Settings> settings,
                                   std::shared_ptr<spdlog::logger> console)
        : timeStep{settings->universeSettings->timeStep}, settings{std::move(settings)}, console{std::move(console)} {

}

void AbstractUniverse::addBody(fp &&mass, Vector3 &&position, Vector3 &&velocity) {
    addBody(std::move(mass), std::move(position), std::move(velocity), {0, 0, 0});
}

void AbstractUniverse::initBodies(std::shared_ptr<UniverseShape> shape) {
    shape->fill(this, settings->universeSettings->number_bodies);
}
