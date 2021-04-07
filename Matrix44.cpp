#include "Matrix44.h"
#include <algorithm>

Matrix44::Matrix44():
        matrix{ {1., 0., 0., 0.},
                {0., 1., 0., 0.},
                {0., 0., 1., 0.},
                {0., 0., 0., 1.} }
{}

Matrix44::Matrix44(double r1[4], double r2[4], double r3[4], double r4[3]):
        matrix{ {r1[0], r1[1], r1[2], r1[3]},
                {r2[0], r2[1], r2[2], r2[3]},
                {r3[0], r3[1], r3[2], r3[3]},
                {r4[0], r4[1], r4[2], r4[3]} }
{}

Matrix44::Matrix44(const Matrix44& other):
        matrix{ {other.matrix[0][0], other.matrix[0][1], other.matrix[0][2], other.matrix[0][3]},
                {other.matrix[1][0], other.matrix[1][1], other.matrix[1][2], other.matrix[1][3]},
                {other.matrix[2][0], other.matrix[2][1], other.matrix[2][2], other.matrix[2][3]},
                {other.matrix[3][0], other.matrix[3][1], other.matrix[3][2], other.matrix[3][3]} }
{}

void Matrix44::set(int row, int col, double value) {
    matrix[row][col] = value;
}

Matrix44& Matrix44::operator=(const Matrix44& other) {
    if (this != &other) {
        Matrix44 temp(other);
        std::swap(matrix, temp.matrix);
    }
    return *this;
}

Matrix44& Matrix44::operator+=(const Matrix44& other) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            matrix[row][col] += other.matrix[row][col];
        }
    }
    return *this;
}

Matrix44& Matrix44::operator*=(const Matrix44& other) {
    Matrix44 answer = *this * other;
    std::swap(matrix, answer.matrix);
    return *this;
}

Matrix44& Matrix44::operator-=(const Matrix44& other) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            matrix[row][col] -= other.matrix[row][col];
        }
    }
    return *this;
}

Matrix44 Matrix44::operator+(const Matrix44& other) const {
    return Matrix44(*this) += other;
}

Matrix44 Matrix44::operator*(const Matrix44& other) const {
    Matrix44 answer;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            double result = 0;
            for (int i = 0; i < 4; i++) {
                result += (matrix[row][i] * other.matrix[i][col]);
            }
            answer.set(row, col, result);
        }
    }
    return answer;
}

Matrix44 Matrix44::operator-(const Matrix44& other) const {
    return Matrix44(*this) -= other;
}

Vector4 Matrix44::operator*(const Vector4& point) const {
    double a, b, c, d;
    a = b = c = d = 0;
    for (int col = 0; col < 4; col++) {
        a += matrix[0][col] * point[col];
        b += matrix[1][col] * point[col];
        c += matrix[2][col] * point[col];
        d += matrix[3][col] * point[col];
    }
    Vector4 result(a, b, c, d);
    return result;
}

std::string Matrix44::toString() const {
    std::string result = "";
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result += std::to_string(matrix[row][col]) + " ";
        }
        result += '\n';
    }

    return result;
}