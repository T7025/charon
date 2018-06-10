//
// Created by thomas on 2/22/18.
//

#ifndef CHARON_VECTOR3S_H
#define CHARON_VECTOR3S_H


#include <vector>
#include <array>

using Body = ulong;

class Vector3 {
public:
    Vector3() = default;
    Vector3(double x, double y, double z) : x{x}, y{y}, z{z} {};
    double x, y, z;
};

class Vector3s : public std::array<std::vector<double>, 3> {
public:
    Vector3 get(Body body) {
        return Vector3{(*this)[0][body], (*this)[1][body], (*this)[2][body]};
    };

    void set(Body body, Vector3 vector) {
        (*this)[0][body] = vector.x;
        (*this)[1][body] = vector.y;
        (*this)[2][body] = vector.z;
    }
private:
};



class FakePosition {
public:
    FakePosition operator+=(const FakePosition other) {

    }

private:
    ulong body;
};


#endif //CHARON_VECTOR3S_H
