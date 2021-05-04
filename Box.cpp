//
// Created by chang on 5/2/2021.
//

#include <limits>
#include <cmath>
#include "Box.h"

Box::Box(const Vector3& lowerLeft, const Vector3& topRight, const Vector3& aColor, const Vector3& rColor):
        lowerLeft(lowerLeft), topRight(topRight), aColor(aColor), rColor(rColor) {}

double Box::hit(Ray ray, double t0) {
    double tmin = (lowerLeft[2] - ray.start[2]) / ray.direction[2];
    double tmax = (topRight[2] - ray.start[2]) / ray.direction[2];

    if (tmin > tmax) std::swap(tmin, tmax);

    double tymin = (lowerLeft[1] - ray.start[1]) / ray.direction[1];
    double tymax = (topRight[1] - ray.start[1]) / ray.direction[1];

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    double tzmin = (lowerLeft[0] - ray.start[0]) / ray.direction[0];
    double tzmax = (topRight[0] - ray.start[0]) / ray.direction[0];

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    return tmin > t0 ? tmin : -1;
}

Vector3 Box::normal(Ray ray, double t) {
    Vector3 point = ray.start + ray.direction*t;
    double minDistance = std::numeric_limits<double>::max();
    Vector3 normal;

    for (int i = 0; i < 3; i++) {
        double distance = fabs(lowerLeft[i] - point[i]);
        if (distance < minDistance) {
            minDistance = distance;
            Vector3 cardinalAxis;
            cardinalAxis[i] = -1;
            normal = cardinalAxis;
        }

        distance = fabs(topRight[i] - point[i]);
        if (distance < minDistance) {
            minDistance = distance;
            Vector3 cardinalAxis;
            cardinalAxis[i] = 1;
            normal = cardinalAxis;
        }
    }

    return normal;
}

Vector3& Box::getAColor() {
    return aColor;
}
Vector3& Box::getRColor() {
    return rColor;
}
Vector3 Box::getSColor() {
    return Vector3();
}

bool Box::specular() {
    return false;
}
