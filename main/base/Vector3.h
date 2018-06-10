//
// Created by thomas on 2/22/18.
//

#ifndef CHARON_VECTOR3_H
#define CHARON_VECTOR3_H


#include <ostream>
#include "Util.h"

class Vector3 {
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

Vector3 operator/(Vector3 lhs, fp scalar);

std::ostream &operator<<(std::ostream &out, const Vector3 &vector);


#endif //CHARON_VECTOR3_H
