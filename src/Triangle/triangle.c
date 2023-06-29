#include "triangle.h"

void draw_filled_triangle(vec2_t* point0, vec2_t* point1, vec2_t* point2, uint32_t color) {
    //Sort the vertices according to their y-coordinate (y0 < y1 < y2)
    if(point0->y > point1->y){
        vec2_swap(point0, point1);
    }
    if(point1->y > point2->y){
        vec2_swap(point1, point2);
    }
    if(point0->y > point1->y) {
        vec2_swap(point0, point1);
    }

    //Calculate the midpoint (Mx,My)
    vec2_t midpoint = calculate_triangle_midpoint(point0, point1, point2);

    //Draw flat-bottom triangle
    fill_flat_bottom_triangle(point0, point1, &midpoint);

    //Draw flat-top triangle
    fill_flat_top_triangle(point0, &midpoint, point1);
}

vec2_t calculate_midpoint(vec2_t* point0, vec2_t* point1, vec2_t* point2) {
    vec2_t midpoint = {
        .x = ( ((point1->y - point0->y) * (point2->x - point0->x)) / (point2->y - point0->y) ) + point0->x,
        .y = point1->y
    };
    return midpoint;
}

void fill_flat_bottom_triangle(vec2_t* point0, vec2_t* point1, vec2_t* midpoint) {

}

void fill_flat_top_triangle(vec2_t* point0, vec2_t* midpoint, vec2_t* point2) {

}

void int_swap(int* a, int* b) {
  int temp = *a;
  *a= *b;
  *b = temp;
}