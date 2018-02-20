//
// Created by thomas on 2/20/18.
//

#ifndef CHARON_VECTOR2D_H
#define CHARON_VECTOR2D_H


class Vector2 {
public:
    Vector2();

    Vector2(double x, double y);

    Vector2 &operator+=(const Vector2 &other);

    Vector2 &operator-=(const Vector2 &other);

    Vector2 &operator*=(const Vector2 &other);

    Vector2 &operator/=(const Vector2 &other);

    Vector2 &operator*=(double scalar);

    Vector2 &operator/=(double scalar);

    bool operator==(const Vector2 &rhs) const;

    bool operator!=(const Vector2 &rhs) const;

    double getX() const;

    double getY() const;

private:
    double x;
    double y;
};

Vector2 operator+(Vector2 lhs, const Vector2 &rhs);

Vector2 operator-(Vector2 lhs, const Vector2 &rhs);

Vector2 operator*(Vector2 lhs, const Vector2 &rhs);

Vector2 operator/(Vector2 lhs, const Vector2 &rhs);

Vector2 operator*(Vector2 lhs, double scalar);

Vector2 operator/(Vector2 lhs, double scalar);

#endif //CHARON_VECTOR2D_H
