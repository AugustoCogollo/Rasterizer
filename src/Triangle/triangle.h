#pragma once

#include "../Vectors/vector.h"

typedef struct {
    int a;
    int b;
    int c;
} face_t;

//This struct stores the actual vec2 points of the triangle on the screen
typedef struct {
    vec2_t points[3];

} triangle_t;