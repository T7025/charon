//
// Created by thomas on 2/19/18.
//

#include "Position.h"

Position &Position::operator+=(const Position &other) {
    x += other.x;
    y += other.y;
    return *this;
}

Position &Position::operator-=(const Position &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Position &Position::operator*=(const Position &other) {
    x *= other.x;
    y *= other.y;
    return *this;
}

Position &Position::operator/=(const Position &other) {
    x /= other.x;
    y /= other.y;
    return *this;
}

Position &Position::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Position &Position::operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

Position operator+(Position lhs, const Position &rhs) {
    return lhs += rhs;
}

Position operator-(Position lhs, const Position &rhs) {
    return lhs -= rhs;
}

Position operator*(Position lhs, const Position &rhs) {
    return lhs *= rhs;
}

Position operator/(Position lhs, const Position &rhs) {
    return lhs /= rhs;
}

Position operator*(Position lhs, double scalar) {
    return lhs *= scalar;
}

Position operator/(Position lhs, double scalar) {
    return lhs /= scalar;
}
