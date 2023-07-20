#include <math.h>
#include "triangle.h"
#include "../DArray/array.h"
#include "../Display/display.h"
#include "../Helper/Swap/swap.h"

void draw_filled_triangle(triangle_t* triangle) {
    //Sort the vertices according to their y-coordinate (y0 < y1 < y2)
    if(triangle->points[0].y > triangle->points[1].y){
        vec4_swap(&triangle->points[0], &triangle->points[1]);
    }
    if(triangle->points[1].y > triangle->points[2].y){
        vec4_swap(&triangle->points[1], &triangle->points[2]);
    }
    if(triangle->points[0].y > triangle->points[1].y){
        vec4_swap(&triangle->points[0], &triangle->points[1]);
    }

    if(triangle->points[1].y == triangle->points[2].y) {
        fill_flat_bottom_triangle(&triangle->points[0], &triangle->points[1], &triangle->points[2], triangle->color);
    }
    else if(&triangle->points[0].y == &triangle->points[1].y) {
        fill_flat_top_triangle(&triangle->points[0], &triangle->points[1], &triangle->points[2], triangle->color); 
    }

    else {
        //Calculate the midpoint (Mx,My)
        vec4_t midpoint = calculate_triangle_midpoint(&triangle->points[0], &triangle->points[1], &triangle->points[2]);

        //Draw flat-bottom triangle
        fill_flat_bottom_triangle(&triangle->points[0], &triangle->points[1], &midpoint, triangle->color);

        //Draw flat-top triangle
        fill_flat_top_triangle(&triangle->points[1], &midpoint, &triangle->points[2], triangle->color);
    }
}

vec4_t calculate_triangle_midpoint(vec4_t* point0, vec4_t* point1, vec4_t* point2) {
    vec4_t midpoint = {
        .x = ( ((point2->x - point0->x) * (point1->y - point0->y)) / (point2->y - point0->y) ) + point0->x,
        .y = point1->y,
        .z = 0,
        .w = 0
    };
    return midpoint;
}

void fill_flat_bottom_triangle(vec4_t* point0, vec4_t* point1, vec4_t* midpoint, uint32_t color) {
    //Change in x per y = inverse slope
    float inverse_slope_start = ((point1->x - point0->x) / (point1->y - point0->y));
    float inverse_slope_end = ((midpoint->x - point0->x) / (midpoint->y - point0->y));

    float x_start = point0->x;
    float x_end = point0->x;

    float max_width = fabsf(midpoint->x - point1->x);
    for(size_t y = point0->y; y <= midpoint->y; y++) {
        //Prevent x_start and x_end from getting out of bounds 
        if(fabsf(x_end - x_start) >= max_width) {
            x_start = point1->x;
            x_end = midpoint->x;
        }

        vec4_t point_start = { roundf(x_start), roundf(y), 0, 0 };
        vec4_t point_end =   { roundf(x_end),   roundf(y), 0, 0 };
        draw_line(&point_start, &point_end, color);
        x_start += inverse_slope_start;
        x_end += inverse_slope_end;

    }
}

void fill_flat_top_triangle(vec4_t* point1, vec4_t* midpoint, vec4_t* point2, uint32_t color) {
    float inverse_slope_start = ((point2->x - point1->x) / (point2->y - point1->y));
    float inverse_slope_end = ((point2->x - midpoint->x) / (point2->y - midpoint->y));

    float x_start = point2->x;
    float x_end = point2->x;

    float max_width = fabsf(midpoint->x - point1->x);
    for(size_t y = point2->y; y >= midpoint->y; y--) {
        if(fabsf(x_end - x_start) > max_width){
            x_start = point1->x;
            x_end = midpoint->x;
        }
        
        vec4_t point_start = { roundf(x_start), roundf(y), 0, 0 };
        vec4_t point_end =   { roundf(x_end),   roundf(y), 0, 0 };
        draw_line(&point_start, &point_end, color);
        x_start -= inverse_slope_start;
        x_end -= inverse_slope_end;

    }
}

void triangle_descending_bubble_sort(triangle_t* triangles) {
    int size = array_length(triangles);
    for(size_t step = 0; step < size; step++) {
        for(size_t j = 0; j < size - step; j++) {
            if(j + 1 >= size){
                break;
            }

            if(triangles[j].avg_depth < triangles[j + 1].avg_depth){
                triangle_swap(&triangles[j], &triangles[j + 1]);
            }
        }
    }
}

void draw_textured_triangle(triangle_t* triangle, uint32_t* texture) {
    //Sort the vertices according to their y-coordinate (y0 < y1 < y2)
    if(triangle->points[0].y > triangle->points[1].y) {
        vec4_swap(&triangle->points[0], &triangle->points[1]);
        tex2_swap(&triangle->tex_coords[0], &triangle->tex_coords[1]);
    }
    if(triangle->points[1].y > triangle->points[2].y) {
        vec4_swap(&triangle->points[1], &triangle->points[2]);
        tex2_swap(&triangle->tex_coords[1], &triangle->tex_coords[2]);
    }
    if(triangle->points[0].y > triangle->points[1].y) {
        vec4_swap(&triangle->points[0], &triangle->points[1]);
        tex2_swap(&triangle->tex_coords[0], &triangle->tex_coords[1]);
    }

    //Render upper part of the triangle (flat-bottom)
    fill_textured_bottom_triangle(triangle, texture);

    //Render bottom part of the triangle (flat-top)
    fill_textured_top_triangle(triangle, texture);
}

void fill_textured_bottom_triangle(triangle_t* triangle, uint32_t* texture) {
    float left_inverse_slope  = 0;
    float right_inverse_slope = 0;

    if(triangle->points[1].y - triangle->points[0].y != 0)
        left_inverse_slope = (triangle->points[1].x - triangle->points[0].x) / abs(triangle->points[1].y - triangle->points[0].y);

    if(triangle->points[2].y - triangle->points[0].y != 0)
        right_inverse_slope = (triangle->points[2].x - triangle->points[0].x) / abs(triangle->points[2].y - triangle->points[0].y);

    if(triangle->points[1].y - triangle->points[0].y != 0)
        for(size_t y = triangle->points[0].y; y <= triangle->points[1].y; y++) {
            int x_start = triangle->points[1].x + (y - triangle->points[1].y) * left_inverse_slope;
            int x_end =   triangle->points[0].x + (y - triangle->points[0].y) * right_inverse_slope;

            if(x_end < x_start) 
                int_swap(&x_start, &x_end);

            for(size_t x = x_start; x < x_end; x++) {
                draw_texel(x, y, triangle, texture);
            }
        }
}

void fill_textured_top_triangle(triangle_t* triangle, uint32_t* texture) {
    float left_inverse_slope  = 0;
    float right_inverse_slope = 0;

    if(triangle->points[2].y - triangle->points[1].y != 0)
        left_inverse_slope = (triangle->points[2].x - triangle->points[1].x) / fabsf(triangle->points[2].y - triangle->points[1].y);

    if(triangle->points[2].y - triangle->points[0].y != 0)
        right_inverse_slope = (triangle->points[2].x - triangle->points[0].x) / fabsf(triangle->points[2].y - triangle->points[0].y);

    if(triangle->points[2].y - triangle->points[1].y != 0)
        for(size_t y = triangle->points[1].y; y <= triangle->points[2].y; y++) {
            int x_start = (int)triangle->points[1].x + (y - triangle->points[1].y) * left_inverse_slope;
            int x_end =   (int)triangle->points[0].x + (y - triangle->points[0].y) * right_inverse_slope;

            if(x_end < x_start) int_swap(&x_start, &x_end);

            for(size_t x = x_start; x < x_end; x++) {
                draw_texel(x, y, triangle, texture);
            }
        }
}