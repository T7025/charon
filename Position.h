//
// Created by thomas on 2/19/18.
//

#ifndef CHARON_POSITION_H
#define CHARON_POSITION_H


class Position {
public:
    Position &operator+=(const Position &other);

    Position &operator-=(const Position &other);

    Position &operator*=(const Position &other);

    Position &operator/=(const Position &other);

    Position &operator*=(double scalar);

    Position &operator/=(double scalar);

private:
    double x;
    double y;
};

Position operator+(Position lhs, const Position &rhs);

Position operator-(Position lhs, const Position &rhs);

Position operator*(Position lhs, const Position &rhs);

Position operator/(Position lhs, const Position &rhs);

Position operator*(Position lhs, double scalar);

Position operator/(Position lhs, double scalar);

#endif //CHARON_POSITION_H
