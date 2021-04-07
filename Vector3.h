#ifndef CS3258_CMAKE_VECTOR3_H
#define CS3258_CMAKE_VECTOR3_H

#include <string>

class Vector3 {
public:
    Vector3();
    Vector3(double a, double b, double c);
    Vector3(const Vector3& other);

    Vector3& operator=(const Vector3& other);

    const double operator[](int index) const;
    double& operator[](int index);

    double dot(const Vector3& other) const;
    double operator*(const Vector3& other) const;
    Vector3 operator*(double scalar);
    Vector3 operator+(const Vector3& other);
    Vector3 operator-(const Vector3& other);
    Vector3 operator-();

    Vector3& operator*=(double scalar);
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);

    bool operator==(const Vector3& other);
    bool operator!=(const Vector3& other);

    Vector3 cross(const Vector3& other) const;
    Vector3 unit() const;

    std::string toString() const;

private:
    double vec[3];
};


#endif //CS3258_CMAKE_VECTOR3_H
