#include "Vector4.h"
#include <algorithm>

Vector4::Vector4(): vec{0, 0, 0, 0} {}

Vector4::Vector4(double a, double b, double c, double d): vec{a, b, c, d} {}

Vector4::Vector4(const Vector4& other): vec{other[0], other[1], other[2], other[3]} {}

Vector4& Vector4::operator=(const Vector4& other) {
    if (this != &other) {
        Vector4 temp(other);
        std::swap(vec, temp.vec);
    }
    return *this;
}

double& Vector4::operator[](int index) {
    return vec[index];
}

const double Vector4::operator[](int index) const {
    return vec[index];
}

Vector4& Vector4::operator*=(double scalar) {
    for (int i = 0; i < 4; i++) {
        vec[i] *= scalar;
    }
    return *this;
}
Vector4& Vector4::operator+=(double scalar) {
    for (int i = 0; i < 4; i++) {
        vec[i] += scalar;
    }
    return *this;
}

std::string Vector4::toString() const {
    std::string result = "[ ";
    for (int i = 0; i < 4; i++) {
        result += std::to_string(vec[i]) + ' ';
    }
    result += ']';
    return result;
}
