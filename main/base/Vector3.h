//
// Created by thomas on 2/22/18.
//

#ifndef CHARON_VECTOR3_H
#define CHARON_VECTOR3_H


#include <ostream>
#include "Util.h"
#include <tuple>
#include <cmath>
#include <iomanip>

#pragma omp declare target
class Vector3 {
public:
    Vector3() = default;
    Vector3(fp x, fp y, fp z) : x{x}, y{y}, z{z} {}

    Vector3 &operator+=(const Vector3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    };

    Vector3 &operator-=(const Vector3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3 &operator*=(const Vector3 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vector3 &operator/=(const Vector3 &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    Vector3 &operator*=(fp scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3 &operator/=(fp scalar) {
        const auto inverse = fp(1.0)/scalar;
        x *= inverse;
        y *= inverse;
        z *= inverse;
        return *this;
    }

    bool operator==(const Vector3 &rhs) const {
        return std::tie(x, y, z) == std::tie(rhs.x, rhs.y, rhs.z);
    }

    bool operator!=(const Vector3 &rhs) const {
        return !(rhs == *this);
    }

    fp norm() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vector3 &normalize() {
        return *this /= norm();
    }

    fp squareDistance(const Vector3 &rhs) const {
        return (x - rhs.x) * (x - rhs.x)
               + (y - rhs.y) * (y - rhs.y)
               + (z - rhs.z) * (z - rhs.z);
    }

    fp distance(const Vector3 &rhs) const {
        return sqrt(squareDistance(rhs));
    }

    Vector3 &min(const Vector3 &rhs) {
        x = std::min(x, rhs.x);
        y = std::min(y, rhs.y);
        z = std::min(z, rhs.z);
        return *this;
    }

    Vector3 &max(const Vector3 &rhs) {
        x = std::max(x, rhs.x);
        y = std::max(y, rhs.y);
        z = std::max(z, rhs.z);
        return *this;
    }

    Vector3 operator+(const Vector3 &rhs) const {
        auto result = *this;
        return result += rhs;
    }

    Vector3 operator-(const Vector3 &rhs) const {
        auto result = *this;
        return result -= rhs;
    }
    Vector3 operator*(const Vector3 &rhs) const {
        auto result = *this;
        return result *= rhs;
    }
    Vector3 operator/(const Vector3 &rhs) const {
        auto result = *this;
        return result /= rhs;
    }
    Vector3 operator*(fp scalar) const {
        auto result = *this;
        return result *= scalar;
    }

    /*Vector3 operator*(fp scalar, Vector3 rhs) {
        return rhs *= scalar;
    }*/

    Vector3 operator/(fp scalar) const {
        auto result = *this;
        return result /= scalar;
    }

    fp x;
    fp y;
    fp z;
};
#pragma omp end declare target

/*class Vector3 {
public:
    Vector3() = default;

    Vector3(fp x, fp y, fp z);

    Vector3 &operator+=(const Vector3 &other);

    Vector3 &operator-=(const Vector3 &other);

    Vector3 &operator*=(const Vector3 &other);

    Vector3 &operator/=(const Vector3 &other);

    Vector3 &operator*=(fp scalar);

    Vector3 &operator/=(fp scalar);

    bool operator==(const Vector3 &rhs) const;

    bool operator!=(const Vector3 &rhs) const;

    fp norm() const;

    Vector3 &normalize();

    fp squareDistance(const Vector3 &rhs) const;

    fp distance(const Vector3 &rhs) const;

    fp x;

    fp y;

    fp z;
};

Vector3 operator+(Vector3 lhs, const Vector3 &rhs);

Vector3 operator-(Vector3 lhs, const Vector3 &rhs);

Vector3 operator*(Vector3 lhs, const Vector3 &rhs);

Vector3 operator/(Vector3 lhs, const Vector3 &rhs);

Vector3 operator*(Vector3 lhs, fp scalar);

Vector3 operator*(fp scalar, Vector3 rhs);

Vector3 operator/(Vector3 lhs, fp scalar);*/

std::ostream &operator<<(std::ostream &out, const Vector3 &vector);


#endif //CHARON_VECTOR3_H
