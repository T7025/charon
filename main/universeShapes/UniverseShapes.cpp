//
// Created by thomas on 3/19/18.
//

#include "UniverseShapes.h"

std::shared_ptr<UniverseShape> UniverseShapes::operator()(const std::string &shapeName) const {
    if (shapeName.empty()) {}
    
    
    return std::shared_ptr<UniverseShape>();
}
