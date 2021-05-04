//
// Created by chang on 5/2/2021.
//

#ifndef CS3258_CMAKE_LIGHT_H
#define CS3258_CMAKE_LIGHT_H

#include "Vector3.h"

class Light {
public:
    Light(const Vector3& color, const Vector3& direction);

    Vector3 color, direction;
};


#endif //CS3258_CMAKE_LIGHT_H
