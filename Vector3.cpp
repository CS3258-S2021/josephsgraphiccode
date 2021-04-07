#include "Vector3.h"
#include <algorithm>
#include <cmath>

Vector3::Vector3(): vec{0, 0, 0} {}

Vector3::Vector3(double a, double b, double c): vec{a, b, c} {}

Vector3::Vector3(const Vector3& other): vec{other[0], other[1], other[2]} {}

Vector3& Vector3::operator=(const Vector3& other) {
    if (this != &other) {
        Vector3 temp(other);
        std::swap(vec, temp.vec);
    }
    return *this;
}

const double Vector3::operator[](int index) const {
    return vec[index];
}

double& Vector3::operator[](int index) {
    return vec[index];
}

double Vector3::dot(const Vector3& other) const {
    return vec[0]*other.vec[0] + vec[1]*other.vec[1] + vec[2]*other.vec[2];
}

double Vector3::operator*(const Vector3& other) const {
    return dot(other);
}

Vector3 Vector3::operator*(double scalar) {
    return Vector3(*this) *= scalar;
}

Vector3 Vector3::operator+(const Vector3& other) {
    return Vector3(*this) += other;
}

Vector3 Vector3::operator-(const Vector3& other) {
    return Vector3(*this) -= other;
}

Vector3 Vector3::operator-() {
    return Vector3(-vec[0], -vec[1], -vec[2]);
}

Vector3& Vector3::operator*=(double scalar) {
    for (int i = 0; i < 3; i++) {
        vec[i] *= scalar;
    }
    return *this;
}

Vector3& Vector3::operator+=(const Vector3& other) {
    for (int i = 0; i < 3; i++) {
        vec[i] += other.vec[i];
    }
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) {
    for (int i = 0; i < 3; i++) {
        vec[i] -= other.vec[i];
    }
    return *this;
}

bool Vector3::operator==(const Vector3& other) {
    return vec[0] == other.vec[0] && vec[1] == other.vec[1] && vec[2] == other.vec[2];
}

bool Vector3::operator!=(const Vector3& other) {
    return !(operator==(other));
}

Vector3 Vector3::cross(const Vector3& other) const {
    Vector3 product(vec[1] * other.vec[2] - vec[2] * other.vec[1],
                    vec[2] * other.vec[0] - vec[0] * other.vec[2],
                    vec[0] * other.vec[1] - vec[1] * other.vec[0]);
    return product;
}

Vector3 Vector3::unit() const {
    double length = sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
    Vector3 unit(vec[0]/length, vec[1]/length, vec[2]/length);
    return unit;
}

std::string Vector3::toString() const {
    return "[" + std::to_string(vec[0]) + ", " + std::to_string(vec[1]) + ", " + std::to_string(vec[2]) + "]";
}