#include "vector.h"
#include <math.h>
#include <stdlib.h>

vec3_t vec3_rotate_x(vec3_t* vector, float angle) {
    vec3_t rotated_vector = {
        .x = vector->x,
        .y = (vector->y * cos(angle)) - (vector->z * sin(angle)),
        .z = (vector->y * sin(angle)) + (vector->z * cos(angle))
    };

    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t* vector, float angle) {
    vec3_t rotated_vector = {
        .x = (vector->x * cos(angle)) - (vector->z * sin(angle)),
        .y = vector->y,
        .z = (vector->x * sin(angle)) + (vector->z * cos(angle))
    };

    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t* vector, float angle) {
    vec3_t rotated_vector = {
        .x = (vector->x * cos(angle)) - (vector->y * sin(angle)),
        .y = (vector->x * sin(angle)) + (vector->y * cos(angle)),
        .z = vector->z
    };

    return rotated_vector;
}