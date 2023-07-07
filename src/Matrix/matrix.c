#include "matrix.h"
#include <math.h>

mat4_t mat4_identity(void) {
    mat4_t identity = {{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    }};
    return identity;
}

vec4_t mat4_mult_vec4(mat4_t* matrix, vec4_t* vector) {
    vec4_t new_vector = {
        .x = (matrix->m[0][0] * vector->x) + (matrix->m[0][1] * vector->y) + (matrix->m[0][2] * vector->z) + (matrix->m[0][3] * vector->w),
        .y = (matrix->m[1][0] * vector->x) + (matrix->m[1][1] * vector->y) + (matrix->m[1][2] * vector->z) + (matrix->m[1][3] * vector->w),
        .z = (matrix->m[2][0] * vector->x) + (matrix->m[2][1] * vector->y) + (matrix->m[2][2] * vector->z) + (matrix->m[2][3] * vector->w),
        .w = (matrix->m[3][0] * vector->x) + (matrix->m[3][1] * vector->y) + (matrix->m[3][2] * vector->z) + (matrix->m[3][3] * vector->w)
    };
    return new_vector;
}

mat4_t mat4_mult_mat4(mat4_t* a, mat4_t* b) {
    mat4_t new_matrix;
    for(size_t y = 0; y < 4; y++) {
        for(size_t x = 0; x < 4; x++) {
            new_matrix.m[y][x] = (a->m[y][0] * b->m[0][x]) + (a->m[y][1] * b->m[1][x]) + (a->m[y][2] * b->m[2][x]) + (a->m[y][3] * b->m[3][x]);
        }
    }
    return new_matrix;
}

mat4_t mat4_make_scale(float sx, float sy, float sz) {
    mat4_t mat_scale = mat4_identity();
    mat_scale.m[0][0] = sx;
    mat_scale.m[1][1] = sy;
    mat_scale.m[2][2] = sz;
    return mat_scale;
}

mat4_t mat4_make_translation(float tx, float ty, float tz) {
    mat4_t mat_translation = mat4_identity();
    mat_translation.m[0][3] = tx;
    mat_translation.m[1][3] = ty;
    mat_translation.m[2][3] = tz;
    return mat_translation;
}

mat4_t mat4_make_rotation_x(float angle) {
    float cosine = cosf(angle);
    float sine = sinf(angle);

    mat4_t mat_rotation_x = mat4_identity();
    mat_rotation_x.m[1][1] =  cosine;
    mat_rotation_x.m[1][2] = -sine;
    mat_rotation_x.m[2][1] =  sine;
    mat_rotation_x.m[2][2] =  cosine;
    return mat_rotation_x;
}

mat4_t mat4_make_rotation_y(float angle) {
    float cosine = cosf(angle);
    float sine = sinf(angle);

    mat4_t mat_rotation_y = mat4_identity();
    mat_rotation_y.m[0][0] =  cosine;
    mat_rotation_y.m[0][2] =  sine;
    mat_rotation_y.m[2][0] = -sine;
    mat_rotation_y.m[2][2] =  cosine;
    return mat_rotation_y;
}

mat4_t mat4_make_rotation_z(float angle) {
    float cosine = cosf(angle);
    float sine = sinf(angle);

    mat4_t mat_rotation_z = mat4_identity();
    mat_rotation_z.m[0][0] =  cosine;
    mat_rotation_z.m[0][1] = -sine;
    mat_rotation_z.m[1][0] =  sine;
    mat_rotation_z.m[1][1] =  cosine;
    return mat_rotation_z;
}
