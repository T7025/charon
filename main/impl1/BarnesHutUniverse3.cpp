//
// Created by thomas on 6/9/18.
//

#include "BarnesHutUniverse3.h"

BarnesHutUniverse3::BarnesHutUniverse3(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console)
        : AbstractUniverse(settings, console) {

}

void BarnesHutUniverse3::initBodies(std::shared_ptr<UniverseShape> shape) {
    AbstractUniverse::initBodies(shape);
}
