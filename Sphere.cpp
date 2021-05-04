//
// Created by chang on 5/2/2021.
//

#include "Sphere.h"
#include <cmath>
#include <algorithm>

Sphere::Sphere(double radius, const Vector3& center, const Vector3& aColor, const Vector3& rColor, const Vector3& sColor):
        radius(radius), center(center), aColor(aColor), rColor(rColor), sColor(sColor) {}

double Sphere::hit(Ray ray, double t0) {
    double disc = pow(ray.direction*(ray.start-center), 2) -
                  (ray.direction * ray.direction) *
                  ((ray.start-center)*(ray.start-center) - radius*radius);
    if (disc < 0) {
        return -1;
    } else {
        double tPlus = (-ray.direction*(ray.start-center) + sqrt(disc)) / (ray.direction*ray.direction);
        if (disc == 0) {
            return tPlus >= t0 ? tPlus : -1;
        } else {
            double tMinus = (-ray.direction*(ray.start-center) - sqrt(disc)) / (ray.direction*ray.direction);
            if (tPlus < t0 && tMinus < t0) {
                return -1;
            } else if (tPlus < t0) {
                return tMinus;
            } else if (tMinus < t0) {
                return tPlus;
            } else {
                return std::min(tPlus, tMinus);
            }
        }
    }
}

Vector3 Sphere::normal(Ray ray, double t) {
    Vector3 point = ray.start + ray.direction*t;
    return (point - center).unit();
}

Vector3& Sphere::getAColor() {
    return aColor;
}
Vector3& Sphere::getRColor() {
    return rColor;
}
Vector3 Sphere::getSColor() {
    return sColor;
}
bool Sphere::specular() {
    return sColor != Vector3(0, 0, 0);
}

Sphere::~Sphere() = default;
