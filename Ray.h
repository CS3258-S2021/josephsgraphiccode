//
// Created by chang on 5/2/2021.
//

#ifndef CS3258_CMAKE_RAY_H
#define CS3258_CMAKE_RAY_H

#include "Vector3.h"

class Ray {
public:
    Ray(const Vector3& start, const Vector3& direction);

    Vector3 start, direction;
};


#endif //CS3258_CMAKE_RAY_H
