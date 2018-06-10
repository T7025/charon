//
// Created by thomas on 5/9/18.
//

#ifndef CHARON_VECTOR3OFFLOAD_H
#define CHARON_VECTOR3OFFLOAD_H

#include <ostream>
#include "Util.h"
#include "Vector3.h"
#include <tuple>
#include <cmath>
#include <iomanip>

#pragma omp declare target
class Vector3Offload {
public:
    Vector3Offload() = default;
    Vector3Offload(fp x, fp y, fp z) : x{x}, y{y}, z{z} {
//    #pragma omp target enter data map(to: x, y, z)
    }

    explicit Vector3Offload(const Vector3 &vector3) : x{vector3.x}, y{vector3.y}, z{vector3.z} {}

    Vector3 toVector3() const {
        return {x, y, z};
    }

    Vector3Offload &operator+=(const Vector3Offload &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    };

    Vector3Offload &operator-=(const Vector3Offload &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3Offload &operator*=(const Vector3Offload &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vector3Offload &operator/=(const Vector3Offload &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    Vector3Offload &operator*=(fp scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3Offload &operator/=(fp scalar) {
        const auto inverse = fp(1.0)/scalar;
        x *= inverse;
        y *= inverse;
        z *= inverse;
        return *this;
    }

    bool operator==(const Vector3Offload &rhs) const {
        return std::tie(x, y, z) == std::tie(rhs.x, rhs.y, rhs.z);
    }

    bool operator!=(const Vector3Offload &rhs) const {
        return !(rhs == *this);
    }

    fp norm() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vector3Offload &normalize() {
        return *this /= norm();
    }

    fp squareDistance(const Vector3Offload &rhs) const {
        return (x - rhs.x) * (x - rhs.x)
               + (y - rhs.y) * (y - rhs.y)
               + (z - rhs.z) * (z - rhs.z);
    }

    fp distance(const Vector3Offload &rhs) const {
        return sqrt(squareDistance(rhs));
    }

    Vector3Offload operator+(const Vector3Offload &rhs) const {
        auto result = *this;
        return result += rhs;
    }

    Vector3Offload operator-(const Vector3Offload &rhs) const {
        auto result = *this;
        return result -= rhs;
    }
    Vector3Offload operator*(const Vector3Offload &rhs) const {
        auto result = *this;
        return result *= rhs;
    }
    Vector3Offload operator/(const Vector3Offload &rhs) const {
        auto result = *this;
        return result /= rhs;
    }
    Vector3Offload operator*(fp scalar) const {
        auto result = *this;
        return result *= scalar;
    }

    /*Vector3Offload operator*(fp scalar, Vector3Offload rhs) {
        return rhs *= scalar;
    }*/

    Vector3Offload operator/(fp scalar) const {
        auto result = *this;
        return result /= scalar;
    }

    fp x;
    fp y;
    fp z;
};
#pragma omp end declare target
/*
#pragma omp declare target
Vector3Offload operator+(Vector3Offload lhs, const Vector3Offload &rhs) {
    return lhs += rhs;
}
#pragma omp end declare target

Vector3Offload operator-(Vector3Offload lhs, const Vector3Offload &rhs) {
    return lhs -= rhs;
}

Vector3Offload operator*(Vector3Offload lhs, const Vector3Offload &rhs) {
    return lhs *= rhs;
}

Vector3Offload operator/(Vector3Offload lhs, const Vector3Offload &rhs) {
    return lhs /= rhs;
}

Vector3Offload operator*(Vector3Offload lhs, fp scalar) {
    return lhs *= scalar;
}

Vector3Offload operator*(fp scalar, Vector3Offload rhs) {
    return rhs *= scalar;
}

Vector3Offload operator/(Vector3Offload lhs, fp scalar) {
    return lhs /= scalar;
}

Vector3Offload operator/(fp scalar, Vector3Offload rhs) {
    return rhs /= scalar;
}
*/

/*
Vector3Offload operator+(Vector3Offload lhs, const Vector3Offload &rhs);

Vector3Offload operator-(Vector3Offload lhs, const Vector3Offload &rhs);

Vector3Offload operator*(Vector3Offload lhs, const Vector3Offload &rhs);

Vector3Offload operator/(Vector3Offload lhs, const Vector3Offload &rhs);

Vector3Offload operator*(Vector3Offload lhs, fp scalar);

Vector3Offload operator*(fp scalar, Vector3Offload rhs);

Vector3Offload operator/(Vector3Offload lhs, fp scalar);

Vector3Offload operator/(fp scalar, Vector3Offload rhs);

*/
std::ostream &operator<<(std::ostream &out, const Vector3Offload &vector);

#endif //CHARON_VECTOR3OFFLOAD_H
