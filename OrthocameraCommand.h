//
// Created by chang on 5/2/2021.
//

#ifndef CS3258_CMAKE_ORTHOCAMERACOMMAND_H
#define CS3258_CMAKE_ORTHOCAMERACOMMAND_H

#include "Camera.h"
#include "Ray.h"

class OrthocameraCommand : public Camera {
public:
    virtual Ray getRay(double x, double y);
    virtual ~OrthocameraCommand();

};


#endif //CS3258_CMAKE_ORTHOCAMERACOMMAND_H
