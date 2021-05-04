//
// Created by chang on 5/2/2021.
//

#ifndef CS3258_CMAKE_SPHERE_H
#define CS3258_CMAKE_SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
    Sphere(double radius, const Vector3& center, const Vector3& aColor, const Vector3& rColor, const Vector3& sColor);
    double hit(Ray ray, double t0) override;
    Vector3 normal(Ray ray, double t) override;
    Vector3& getAColor() override;
    Vector3& getRColor() override;
    Vector3 getSColor() override;
    bool specular() override;
    ~Sphere() override;

private:
    double radius;
    Vector3 center;
    Vector3 aColor, rColor, sColor;
};


#endif //CS3258_CMAKE_SPHERE_H
