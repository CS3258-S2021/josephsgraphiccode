//
// Created by chang on 5/2/2021.
//

#include "Light.h"

Light::Light(const Vector3& color, const Vector3& direction) : color(color), direction(direction.unit()) {}