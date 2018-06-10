//
// Created by thomas on 6/2/18.
//

#include "SFCIndex.h"

std::ostream &operator<<(std::ostream &out, const SFCIndex &sfcIndex) {
    out << std::bitset<k>{sfcIndex.x} << "," << std::bitset<k>{sfcIndex.y} << "," << std::bitset<k>{sfcIndex.z};
    return out;
}
