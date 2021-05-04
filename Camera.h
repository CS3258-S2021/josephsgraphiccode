//
// Created by chang on 5/2/2021.
//

#ifndef CS3258_CMAKE_CAMERA_H
#define CS3258_CMAKE_CAMERA_H

#include "Ray.h"

class Camera {
public:
    virtual Ray getRay(double x, double y) = 0;
    virtual ~Camera() {};
};


#endif //CS3258_CMAKE_CAMERA_H
