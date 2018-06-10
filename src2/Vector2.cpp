//
// Created by thomas on 2/20/18.
//

#include <tuple>
#include <cmath>
#include "Vector2.h"


Vector2::Vector2() : x{}, y{} {

}

Vector2::Vector2(double x, double y) : x{x}, y{y} {

}

Vector2 &Vector2::operator+=(const Vector2 &other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2 &Vector2::operator-=(const Vector2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2 &Vector2::operator*=(const Vector2 &other) {
    x *= other.x;
    y *= other.y;
    return *this;
}

Vector2 &Vector2::operator/=(const Vector2 &other) {
    x /= other.x;
    y /= other.y;
    return *this;
}

Vector2 &Vector2::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2 &Vector2::operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

bool Vector2::operator==(const Vector2 &rhs) const {
    return std::tie(x, y) == std::tie(rhs.x, rhs.y);
}

bool Vector2::operator!=(const Vector2 &rhs) const {
    return !(rhs == *this);
}

Vector2 &Vector2::normalize() {
    return *this /= sqrt(x*x + y*y);
}

double Vector2::norm() const {
    return x*x + y*y;
}

double Vector2::distance(const Vector2 &rhs) const {
    return sqrt(distance2(rhs));
}

double Vector2::distance2(const Vector2 &rhs) const {
    return pow(x - rhs.x, 2) + pow(y - rhs.y, 2);
}

Vector2 operator+(Vector2 lhs, const Vector2 &rhs) {
    return lhs += rhs;
}

Vector2 operator-(Vector2 lhs, const Vector2 &rhs) {
    return lhs -= rhs;
}

Vector2 operator*(Vector2 lhs, const Vector2 &rhs) {
    return lhs *= rhs;
}

Vector2 operator/(Vector2 lhs, const Vector2 &rhs) {
    return lhs /= rhs;
}

Vector2 operator*(Vector2 lhs, double scalar) {
    return lhs *= scalar;
}

Vector2 operator*(double scalar, Vector2 rhs) {
    return rhs *= scalar;
}

Vector2 operator/(Vector2 lhs, double scalar) {
    return lhs /= scalar;
}

std::ostream &operator<<(std::ostream &out, const Vector2 &vector) {
    out << vector.x << ", " << vector.y;
    return out;
}
