#pragma once 

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

float   vec2_length(vec2_t* vector);
vec2_t* vec2_add(vec2_t* a, vec2_t* b);
vec2_t* vec2_sub(vec2_t* a, vec2_t* b);
void    vec2_mult(vec2_t* vector, float scalar);
void    vec2_div(vec2_t* vector, float factor);
float   vec2_cross(vec2_t* a, vec2_t* b);

float   vec3_length(vec3_t* vector);
vec3_t* vec3_add(vec3_t* a, vec3_t* b);
vec3_t* vec3_sub(vec3_t* a, vec3_t* b);
void    vec3_mult(vec3_t* vector, float scalar);
void    vec3_div(vec3_t* vector, float factor);
vec3_t* vec3_cross(vec3_t* a, vec3_t* b);
float   vec3_dot(vec3_t* a, vec3_t* b);
vec3_t  vec3_rotate_x(vec3_t* vector, float angle);
vec3_t  vec3_rotate_y(vec3_t* vector, float angle);
vec3_t  vec3_rotate_z(vec3_t* vector, float angle);
