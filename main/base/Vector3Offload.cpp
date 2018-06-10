//
// Created by thomas on 5/9/18.
//

#include <tuple>
#include <cmath>
#include <iomanip>
#include "Vector3Offload.h"

//#pragma omp declare target
//Vector3Offload::Vector3Offload(fp x, fp y, fp z) : x{x}, y{y}, z{z} {
//    #pragma omp target enter data map(to: x, y, z)
//}

/*#pragma omp declare target
Vector3Offload &Vector3Offload::operator+=(const Vector3Offload &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}
#pragma omp end declare target*/

/*
Vector3Offload &Vector3Offload::operator-=(const Vector3Offload &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3Offload &Vector3Offload::operator*=(const Vector3Offload &other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}

Vector3Offload &Vector3Offload::operator/=(const Vector3Offload &other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
}

Vector3Offload &Vector3Offload::operator*=(fp scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3Offload &Vector3Offload::operator/=(fp scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

bool Vector3Offload::operator==(const Vector3Offload &rhs) const {
    return std::tie(x, y, z) == std::tie(rhs.x, rhs.y, rhs.z);
}

bool Vector3Offload::operator!=(const Vector3Offload &rhs) const {
    return !(rhs == *this);
}

fp Vector3Offload::norm() const {
    return sqrt(x * x + y * y + z * z);
}

Vector3Offload &Vector3Offload::normalize() {
    return *this /= norm();
}

fp Vector3Offload::squareDistance(const Vector3Offload &rhs) const {
    return (x - rhs.x) * (x - rhs.x)
           + (y - rhs.y) * (y - rhs.y)
           + (z - rhs.z) * (z - rhs.z);
}

fp Vector3Offload::distance(const Vector3Offload &rhs) const {
    return sqrt(squareDistance(rhs));
}

Vector3Offload operator+(Vector3Offload lhs, const Vector3Offload &rhs) {
    return lhs += rhs;
}

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
std::ostream &operator<<(std::ostream &out, const Vector3Offload &vector) {
    out << vector.x << "," << vector.y << "," << vector.z;
    return out;
}
