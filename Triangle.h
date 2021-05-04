//
// Created by chang on 5/2/2021.
//

#ifndef CS3258_CMAKE_TRIANGLE_H
#define CS3258_CMAKE_TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape {
public:
    Triangle(Vector3 u, Vector3 v, Vector3 w, Vector3 aColor, Vector3 rColor);
    virtual double hit(Ray ray, double t0);
    virtual Vector3 normal(Ray ray, double t);
    virtual Vector3& getAColor();
    virtual Vector3& getRColor();
    virtual Vector3 getSColor();
    virtual bool specular();
    virtual ~Triangle();

private:
    Vector3 av, bv, cv;
    Vector3 aColor, rColor;
};


#endif //CS3258_CMAKE_TRIANGLE_H
