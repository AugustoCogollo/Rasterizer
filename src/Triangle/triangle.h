#pragma once

#include <stdint.h>
#include "../Vectors/vector.h" 
#include "../Texture/texture.h"

typedef struct {
    int a;
    int b;
    int c;
    tex2_t a_uv;
    tex2_t b_uv;
    tex2_t c_uv;
    int a_norm;
    int b_norm;
    int c_norm;
    uint32_t color;
} face_t;

//This struct stores the actual vec2 points of the triangle on the screen
typedef struct {
    vec4_t points[3];
    tex2_t tex_coords[3];
    uint32_t color;
    float avg_depth;
} triangle_t;

void draw_filled_triangle(triangle_t* triangle);
vec4_t calculate_triangle_midpoint(vec4_t* point0, vec4_t* point1, vec4_t* point);
void fill_flat_bottom_triangle(vec4_t* point0, vec4_t* point1, vec4_t* midpoint, uint32_t color);
void fill_flat_top_triangle(vec4_t* point0, vec4_t* midpoint, vec4_t* point2, uint32_t color);

void triangle_descending_bubble_sort(triangle_t* triangles);

void draw_textured_triangle(triangle_t* triangle, const uint32_t* texture);
void fill_textured_bottom_triangle(triangle_t* triangle,const uint32_t* texture);
void fill_textured_top_triangle(triangle_t* triangle, const uint32_t* texture);