#include "vector.h"
#include <math.h>
#include <stdlib.h>

float vec2_length(vec2_t* vector) {
    return sqrt((vector->x * vector->x) + (vector->y * vector->y));
}

vec2_t* vec2_add(vec2_t* a, vec2_t* b) {
    //Compound literal
    vec2_t* result = &(vec2_t) {
        .x = a->x + b->x,
        .y = a->y + b->y
    };
    return result;
}

vec2_t* vec2_sub(vec2_t* a, vec2_t* b) {
    vec2_t* result = &(vec2_t) {
        .x = a->x - b->x,
        .y = a->y - b->y
    };
    return result;
}

void vec2_mult(vec2_t* vector, float scalar) {
    vector->x *= scalar;
    vector->y *= scalar;
}

void vec2_div(vec2_t* vector, float factor) {
    vector->x /= factor;
    vector->y /= factor;
}

float vec2_dot(vec2_t* a, vec2_t* b) {
    return (a->x * b->x) + (a->y * b->y);
}

void vec2_normalize(vec2_t* vector) {
    float magnitude = vec2_length(vector);
    vec2_div(vector, magnitude);
}

float vec3_length(vec3_t* vector) {
    return sqrt((vector->x * vector->x) + (vector->y * vector->y) + (vector->z * vector->z));
}

vec3_t vec3_add(vec3_t* a, vec3_t* b) { 
    vec3_t result = {
        .x = a->x + b->x,
        .y = a->y + b->y,
        .z = a->z + b->z
    };
    return result;
}

vec3_t vec3_sub(vec3_t* a, vec3_t* b) {
    vec3_t result = {
        .x = a->x - b->x,
        .y = a->y - b->y,
        .z = a->z - b->z
    };
    return result;
}

void vec3_mult(vec3_t* vector, float scalar) {
    vector->x *= scalar;
    vector->y *= scalar;
    vector->z *= scalar;
}

void vec3_div(vec3_t* vector, float factor) {
    vector->x /= factor;
    vector->y /= factor;
    vector->z /= factor;
}

vec3_t vec3_cross(vec3_t* a, vec3_t* b) {
    vec3_t result = {
        .x = (a->y * b->z) - (a->z * b->y),
        .y = (a->z * b->x) - (a->x * b->z),
        .z = (a->x * b->y) - (a->y * b->x)
    };

    return result;
}

float vec3_dot(vec3_t* a, vec3_t* b) {
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

void vec3_normalize(vec3_t* vector) {
    float magnitude = vec3_length(vector);
    vec3_div(vector, magnitude);
}

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

vec4_t* vec4_add(vec4_t* a, vec4_t* b) {
    vec4_t* result = &(vec4_t) {
        .x = a->x + b->x,
        .y = a->y + b->y,
        .z = a->z + b->z,
        .w = a->w + b->w
    };
    return result;
}

vec4_t* vec4_sub(vec4_t* a, vec4_t* b) {
    vec4_t* result = &(vec4_t) {
        .x = a->x - b->x,
        .y = a->y - b->y,
        .z = a->z - b->z,
        .w = a->w - b->w
    };
    return result;
}

vec4_t vec4_from_vec3(vec3_t* vector) {
    vec4_t new_vector = { vector->x, vector->y, vector->z, 1.0 };
    return new_vector;
}

vec3_t vec3_from_vec4(vec4_t* vector) {
    vec3_t new_vector = { vector->x, vector->y, vector->z };
    return new_vector;
}

vec2_t vec2_from_vec4(vec4_t* vector) {
    vec2_t new_vector = { vector->x, vector->y };
    return new_vector;
}