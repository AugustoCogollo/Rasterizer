#pragma once

#include <stdint.h>
#include "../../Triangle/triangle.h"
#include "../../Vectors/vector.h"
#include "../../Texture/texture.h"

void triangle_swap(triangle_t* a, triangle_t* b);
void int_swap(int* a, int* b);
void float_swap(float* a, float* b);
void vec4_swap(vec4_t* a, vec4_t* b);
void vec3_swap(vec3_t* a, vec3_t* b);
void vec2_swap(vec2_t* a, vec2_t* b);
void tex2_swap(tex2_t* a, tex2_t* b);
