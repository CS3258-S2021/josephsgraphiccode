//
// Created by chang on 5/2/2021.
//

#include "Triangle.h"

Triangle::Triangle(Vector3 u, Vector3 v, Vector3 w, Vector3 aColor, Vector3 rColor):
        av(u), bv(v), cv(w), aColor(aColor), rColor(rColor) {}

double Triangle::hit(Ray ray, double t0) {
    double a = av[0] - bv[0];
    double b = av[1] - bv[1];
    double c = av[2] - bv[2];

    double d = av[0] - cv[0];
    double e = av[1] - cv[1];
    double f = av[2] - cv[2];

    double g = ray.direction[0];
    double h = ray.direction[1];
    double i = ray.direction[2];

    double j = av[0] - ray.start[0];
    double k = av[1] - ray.start[1];
    double l = av[2] - ray.start[2];

    double ei_hf = e * i - h * f;
    double gf_di = g * f - d * i;
    double dh_eg = d * h - e * g;
    double ak_jb = a * k - j * b;
    double jc_al = j * c - a * l;
    double bl_kc = b * l - k * c;

    double m = a * ei_hf + b * gf_di + c * dh_eg;

    double t = -(f * ak_jb + e * jc_al + d * bl_kc) / m;

    if (t < t0) {
        return -1;
    } else {
        double gamma = (i * ak_jb + h * jc_al + g * bl_kc) / m;
        if (gamma < 0 || gamma > 1) {
            return -1;
        } else {
            double beta = (j * ei_hf + k * gf_di + l * dh_eg) / m;
            if (beta < 0 || beta > 1-gamma) {
                return -1;
            } else {
                return t;
            }
        }
    }
}

Vector3 Triangle::normal(Ray ray, double t) {
    Vector3 n = (bv - av).cross(cv - av);
    return ray.direction*n < 0 ? n : -n;
}

Vector3& Triangle::getAColor() {
    return aColor;
}
Vector3& Triangle::getRColor() {
    return rColor;
}
Vector3 Triangle::getSColor() {
    return Vector3(0, 0, 0);
}
bool Triangle::specular() {
    return false;
}

Triangle::~Triangle() {}