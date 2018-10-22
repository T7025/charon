//
// Created by thomas on 2/22/18.
//

#include <tuple>
#include <cmath>
#include <iomanip>
#include "Vector3.h"


/*Vector3::Vector3(fp x, fp y, fp z) : x{x}, y{y}, z{z} {}

Vector3 &Vector3::operator+=(const Vector3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3 &Vector3::operator-=(const Vector3 &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;}

Vector3 &Vector3::operator*=(const Vector3 &other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;}

Vector3 &Vector3::operator/=(const Vector3 &other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;}

Vector3 &Vector3::operator*=(fp scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3 &Vector3::operator/=(fp scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

bool Vector3::operator==(const Vector3 &rhs) const {
    return std::tie(x, y, z) == std::tie(rhs.x, rhs.y, rhs.z);
}

bool Vector3::operator!=(const Vector3 &rhs) const {
    return !(rhs == *this);
}

fp Vector3::norm() const {
    return sqrt(x*x + y*y + z*z);
}

Vector3 &Vector3::normalize() {
    return *this /= norm();
}

fp Vector3::squareDistance(const Vector3 &rhs) const {
    return (x - rhs.x) * (x - rhs.x)
           + (y - rhs.y) * (y - rhs.y)
           + (z - rhs.z) * (z - rhs.z);
}

fp Vector3::distance(const Vector3 &rhs) const {
    return sqrt(squareDistance(rhs));
}

Vector3 operator+(Vector3 lhs, const Vector3 &rhs) {
    return lhs += rhs;
}

Vector3 operator-(Vector3 lhs, const Vector3 &rhs) {
    return lhs -= rhs;
}

Vector3 operator*(Vector3 lhs, const Vector3 &rhs) {
    return lhs *= rhs;
}

Vector3 operator/(Vector3 lhs, const Vector3 &rhs) {
    return lhs /= rhs;
}

Vector3 operator*(Vector3 lhs, fp scalar) {
    return lhs *= scalar;
}

Vector3 operator*(fp scalar, Vector3 rhs) {
    return rhs *= scalar;
}

Vector3 operator/(Vector3 lhs, fp scalar) {
    return lhs /= scalar;
}*/

std::ostream &operator<<(std::ostream &out, const Vector3 &vector) {
    out << vector.x << "," << vector.y << "," << vector.z;
    return out;
}

