#include "triangle.h"
#include "../DArray/array.h"
#include "../Display/display.h"

void draw_filled_triangle(triangle_t* triangle, uint32_t color) {
    //Sort the vertices according to their y-coordinate (y0 < y1 < y2)
    if(triangle->points[0].y > triangle->points[1].y){
        vec2_swap(&triangle->points[0], &triangle->points[1]);
    }
    if(triangle->points[1].y > triangle->points[2].y){
        vec2_swap(&triangle->points[1], &triangle->points[2]);
    }
    if(triangle->points[0].y > triangle->points[1].y){
        vec2_swap(&triangle->points[0], &triangle->points[1]);
    }

    if(triangle->points[1].y == triangle->points[2].y) {
        fill_flat_bottom_triangle(&triangle->points[0], &triangle->points[1], &triangle->points[2], color);
    }
    else if(&triangle->points[0].y == &triangle->points[1].y) {
        fill_flat_top_triangle(&triangle->points[0], &triangle->points[1], &triangle->points[2], color); 
    }

    else {
        //Calculate the midpoint (Mx,My)
        vec2_t midpoint = calculate_triangle_midpoint(&triangle->points[0], &triangle->points[1], &triangle->points[2]);

        //Draw flat-bottom triangle
        fill_flat_bottom_triangle(&triangle->points[0], &triangle->points[1], &midpoint, color);

        //Draw flat-top triangle
        fill_flat_top_triangle(&triangle->points[1], &midpoint, &triangle->points[2], color);
    }

}

vec2_t calculate_triangle_midpoint(vec2_t* point0, vec2_t* point1, vec2_t* point2) {
    vec2_t midpoint = {
        .x = ( ((point1->y - point0->y) * (point2->x - point0->x)) / (point2->y - point0->y) ) + point0->x,
        .y = point1->y
    };
    return midpoint;
}

void fill_flat_bottom_triangle(vec2_t* point0, vec2_t* point1, vec2_t* midpoint, uint32_t color) {
    //Change in x per y = inverse slope
    float inverse_slope_start = ((point1->x - point0->x) / (point1->y - point0->y));
    float inverse_slope_end = ((midpoint->x - point0->x) / (midpoint->y - point0->y));

    float x_start = point0->x;
    float x_end = point0->x;

    float max_width = fabsf(midpoint->x - point1->x);
    for(size_t y = point0->y; y <= midpoint->y; y++) {
        vec2_t point_start = { x_start, y };
        vec2_t point_end = { x_end, y };
        draw_line(&point_start, &point_end, color);
        x_start += inverse_slope_start;
        x_end += inverse_slope_end;

        //Prevent x_start and x_end from getting out of bounds 
        if(fabsf(x_end - x_start) > max_width) {
            x_start = point1->x;
            x_end = midpoint->x;
        }
    }
}

void fill_flat_top_triangle(vec2_t* point1, vec2_t* midpoint, vec2_t* point2, uint32_t color) {
    float inverse_slope_start = ((point2->x - point1->x) / (point2->y - point1->y));
    float inverse_slope_end = ((point2->x - midpoint->x) / (point2->y - midpoint->y));

    float x_start = point2->x;
    float x_end = point2->x;

    float max_width = fabsf(midpoint->x - point1->x);
    for(size_t y = point2->y; y >= midpoint->y; y--) {
        vec2_t point_start = { x_start, y };
        vec2_t point_end = { x_end, y };
        draw_line(&point_start, &point_end, color);
        x_start -= inverse_slope_start;
        x_end -= inverse_slope_end;

        if(fabsf(x_end - x_start) > max_width){
            x_start = point1->x;
            x_end = midpoint->x;
        }
    }
}

void triangle_descending_bubble_sort(triangle_t* triangles) {
    int size = array_length(triangles);
    for(size_t step = 0; step < size; step++) {
        for(size_t j = 0; j < size - step; j++) {
            if(j + 1 >= size){
                //printf("Reached limit of the array at %d", size);
                break;
            }

            if(triangles[j].avg_depth < triangles[j + 1].avg_depth){
                triangle_swap(&triangles[j], &triangles[j + 1]);
            }
        }
    }
}

void triangle_swap(triangle_t* a, triangle_t* b) {
    triangle_t temp = *a;
    *a = *b;
    *b = temp;
}

void int_swap(int* a, int* b) {
  int temp = *a;
  *a= *b;
  *b = temp;
}