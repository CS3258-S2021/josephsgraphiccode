//
// Created by chang on 5/4/2021.
//

#ifndef CS3258_CMAKE_PERSPCAMERA_H
#define CS3258_CMAKE_PERSPCAMERA_H

#include "Camera.h"
#include "Ray.h"
#include "Vector3.h"
#include "CLI.h"

class PerspCamera : public Camera {
public:
    PerspCamera(const Vector3& eye, const Vector3& gaze, const Vector3& up, double d, double au, double av, double bu, double bv, CLI& cli);
    virtual Ray getRay(double x, double y);
    virtual ~PerspCamera();
private:
    double left, right, top, bottom;
    double d;
    Vector3 eye;
    Vector3 u, v, w;
    CLI cli;
};


#endif //CS3258_CMAKE_PERSPCAMERA_H
