#pragma once

#include <stdint.h>
#include "../Vectors/vector.h"
#include "../Colors/color.h"

typedef struct {
    int a;
    int b;
    int c;
    color_t color;
} face_t;

//This struct stores the actual vec2 points of the triangle on the screen
typedef struct {
    vec2_t points[3];
    color_t color;
    float avg_depth;
} triangle_t;

void draw_filled_triangle(triangle_t* triangle, uint32_t color);
vec2_t calculate_triangle_midpoint(vec2_t* point0, vec2_t* point1, vec2_t* point);
void fill_flat_bottom_triangle(vec2_t* point0, vec2_t* point1, vec2_t* midpoint, uint32_t color);
void fill_flat_top_triangle(vec2_t* point0, vec2_t* midpoint, vec2_t* point2, uint32_t color);
void triangle_descending_bubble_sort(triangle_t* triangles);
void triangle_swap(triangle_t* a, triangle_t* b);
void int_swap(int* a, int* b);