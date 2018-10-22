//
// Created by thomas on 5/28/18.
//

#include "BarnesHutUniverse2.h"

BarnesHutUniverse2::BarnesHutUniverse2(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console)
        : AbstractUniverse(settings, console) {

}

void BarnesHutUniverse2::addBody(fp &&mass, Vector3 &&position, Vector3 &&velocity, Vector3 &&acceleration) {
    this->position.emplace_back(Vector3{position.x + 10, position.y + 10, position.z + 10});
    positionX.emplace_back(position.x);
    positionY.emplace_back(position.y);
    positionZ.emplace_back(position.z);
}

std::vector<Vector3> BarnesHutUniverse2::getPositions() const {
    return position;
}
