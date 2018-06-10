//
// Created by thomas on 2/22/18.
//

#ifndef CHARON_VECTOR3_H
#define CHARON_VECTOR3_H


class Vector3 {
public:
    Vector3();

    Vector3(double x, double y);

    Vector3 &operator+=(const Vector3 &other);

    Vector3 &operator-=(const Vector3 &other);

    Vector3 &operator*=(const Vector3 &other);

    Vector3 &operator/=(const Vector3 &other);

    Vector3 &operator*=(double scalar);

    Vector3 &operator/=(double scalar);

    bool operator==(const Vector3 &rhs) const;

    bool operator!=(const Vector3 &rhs) const;

    Vector3 &normalize();

    double norm() const;

    double distance(const Vector3 &rhs) const;

    // Square of euclidean distance between this vector and the rhs.
    double distance2(const Vector3 &rhs) const;

    double x;

    double y;

    double z;
};


#endif //CHARON_VECTOR3_H
