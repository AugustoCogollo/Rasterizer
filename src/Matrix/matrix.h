#pragma once 

#include "../Vectors/vector.h"

typedef struct {
    float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_make_scale(float sx, float sy, float sz);
mat4_t mat4_make_translation(float tx, float ty, float tz);
mat4_t mat4_make_rotation_x(float angle);
mat4_t mat4_make_rotation_y(float angle);
mat4_t mat4_make_rotation_z(float angle);
mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar);
//TODO mat4_t mat4_make_rotation(float rx, float ry, float rz);

vec4_t mat4_mult_vec4(mat4_t* matrix, vec4_t* vector);
mat4_t mat4_mult_mat4(mat4_t* a, mat4_t* b);
vec4_t mat4_mul_vec4_project(mat4_t* mat_proj, vec4_t* vector);