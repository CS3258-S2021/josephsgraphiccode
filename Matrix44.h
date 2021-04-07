#ifndef MATRIX44_H
#define MATRIX44_H

#include "Vector4.h"
#include <string>

class Matrix44 {
public:
    Matrix44();
    Matrix44(double r1[4], double r2[4], double r3[4], double r4[3]);
    Matrix44(const Matrix44& other);

    Matrix44& operator=(const Matrix44& other);

    void set(int row, int col, double value);

    Matrix44& operator+=(const Matrix44& other);
    Matrix44& operator*=(const Matrix44& other);
    Matrix44& operator-=(const Matrix44& other);

    Matrix44 operator+(const Matrix44& other) const;
    Matrix44 operator*(const Matrix44& other) const;
    Matrix44 operator-(const Matrix44& other) const;

    Vector4 operator*(const Vector4& point) const;

    std::string toString() const;

private:
    double matrix[4][4];
};


#endif //MATRIX44_H
