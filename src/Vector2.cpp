//
// Created by thomas on 2/20/18.
//

#include <tuple>
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

double Vector2::getX() const {
    return x;
}

double Vector2::getY() const {
    return y;
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

Vector2 operator/(Vector2 lhs, double scalar) {
    return lhs /= scalar;
}
