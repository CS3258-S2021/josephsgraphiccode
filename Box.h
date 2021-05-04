//
// Created by chang on 5/2/2021.
//

#ifndef CS3258_CMAKE_BOX_H
#define CS3258_CMAKE_BOX_H

#include "Shape.h"

class Box : public Shape {
public:
    Box(const Vector3& lowerLeft, const Vector3& topRight, const Vector3& aColor, const Vector3& rColor);
    double hit(Ray ray, double t0) override;
    Vector3 normal(Ray ray, double t) override;
    Vector3& getAColor() override;
    Vector3& getRColor() override;
    Vector3 getSColor() override;
    bool specular() override;
    ~Box() override {};

private:
    Vector3 lowerLeft, topRight, aColor, rColor;

    bool isBetweenInclusive(double a, double b, double x);

    bool isBetweenExclusive(double a, double b, double x);
};


#endif //CS3258_CMAKE_BOX_H
