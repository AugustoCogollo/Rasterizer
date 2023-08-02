#include <math.h>
#include <stdio.h>
#include "triangle.h"
#include "../DArray/array.h"
#include "../Display/display.h"
#include "../Helper/Swap/swap.h"
#include "../Helper/Mathr/ren_math.h"

void draw_filled_triangle(triangle_t* triangle) {
    // We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if (triangle->points[0].y > triangle->points[1].y) {
        float_swap(&triangle->points[0].y, &triangle->points[1].y);
        float_swap(&triangle->points[0].x, &triangle->points[1].x);
        float_swap(&triangle->points[0].z, &triangle->points[1].z);
        float_swap(&triangle->points[0].w, &triangle->points[1].w);
    }
    if (triangle->points[1].y > triangle->points[2].y) {
        float_swap(&triangle->points[1].y, &triangle->points[2].y);
        float_swap(&triangle->points[1].x, &triangle->points[2].x);
        float_swap(&triangle->points[1].z, &triangle->points[2].z);
        float_swap(&triangle->points[1].w, &triangle->points[2].w);
    }
    if (triangle->points[0].y > triangle->points[1].y) {
        float_swap(&triangle->points[0].y, &triangle->points[1].y);
        float_swap(&triangle->points[0].x, &triangle->points[1].x);
        float_swap(&triangle->points[0].z, &triangle->points[1].z);
        float_swap(&triangle->points[0].w, &triangle->points[1].w);
    }

    fill_flat_bottom_triangle(&triangle->points[0], &triangle->points[1], &triangle->points[2], triangle->color);
    fill_flat_top_triangle(&triangle->points[0], &triangle->points[1], &triangle->points[2], triangle->color);
}

// Render the upper part of the triangle (flat-bottom)
void fill_flat_bottom_triangle(vec4_t* point0, vec4_t* point1, vec4_t* point2, uint32_t color) {
    float inv_slope_1 = 0;
    float inv_slope_2 = 0;

    if (point1->y - point0->y != 0) inv_slope_1 = (point1->x - point0->x) / abs(point1->y - point0->y);
    if (point2->y - point0->y != 0) inv_slope_2 = (point2->x - point0->x) / abs(point2->y - point0->y);

    if (point1->y - point0->y != 0) {
        for (int y = point0->y; y <= point1->y; y++) {
            int x_start = point1->x + (y - point1->y) * inv_slope_1;
            int x_end = point0->x + (y - point0->y) * inv_slope_2;

            if (x_end < x_start) {
                int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) {
                // Draw our pixel with a solid color
                draw_triangle_pixel(x, y, point0, point1, point2, color);
            }
        }
    }
}

//Render the bottom part of the triangle (flat top)
void fill_flat_top_triangle(vec4_t* point0, vec4_t* point1, vec4_t* point2, uint32_t color) {
    float inv_slope_1 = 0;
    float inv_slope_2 = 0;

    if (point2->y - point1->y != 0) inv_slope_1 = (point2->x - point1->x) / abs(point2->y - point1->y);
    if (point2->y - point0->y != 0) inv_slope_2 = (point2->x - point0->x) / abs(point2->y - point0->y);

    if (point2->y - point1->y != 0) {
        for (int y = point1->y; y <= point2->y; y++) {
            int x_start = point1->x + (y - point1->y) * inv_slope_1;
            int x_end = point0->x + (y - point0->y) * inv_slope_2;

            if (x_end < x_start) {
                int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) {
                draw_triangle_pixel(x, y, point0, point1, point2, color);
            }
        }
    }
}

void draw_triangle_pixel(int x, int y, vec4_t* point_a, vec4_t* point_b, vec4_t* point_c, uint32_t color) {
    // Create three vec2 to find the interpolation
    vec2_t p = { x, y };
    vec2_t a = vec2_from_vec4(point_a);
    vec2_t b = vec2_from_vec4(point_b);
    vec2_t c = vec2_from_vec4(point_c);

    // Calculate the barycentric coordinates of our point 'p' inside the triangle
    vec3_t weights = barycentric_weights(&a, &b, &c, &p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;
    
    // Interpolate the value of 1/w for the current pixel
    float interpolated_reciprocal_w = (1 / point_a->w) * alpha + (1 / point_b->w) * beta + (1 / point_c->w) * gamma;

    // Adjust 1/w so the pixels that are closer to the camera have smaller values
    interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

    // Only draw the pixel if the depth value is less than the one previously stored in the z-buffer
    if (interpolated_reciprocal_w < z_buffer[(window_width * y) + x]) {
        // Draw a pixel at position (x,y) with a solid color
        draw_pixel(x, y, color);

        // Update the z-buffer value with the 1/w of this current pixel
        z_buffer[(window_width * y) + x] = interpolated_reciprocal_w;
    }
}

void triangle_descending_bubble_sort(triangle_t* triangles) {
    int size = array_length(triangles);
    for(size_t step = 0; step < size; step++) {
        for(size_t j = 0; j < size - step; j++) {
            if(j + 1 >= size){
                break;
            }

            // if(triangles[j].avg_depth < triangles[j + 1].avg_depth){
            //     triangle_swap(&triangles[j], &triangles[j + 1]);
            // }
        }
    }
}

// Draw a textured triangle based on a texture array of colors.
void draw_textured_triangle(triangle_t* triangle, const uint32_t* texture) {
    //Sort the vertices according to their y-coordinate (y0 < y1 < y2)
    if (triangle->points[0].y > triangle->points[1].y) {
        vec4_swap(&triangle->points[0], &triangle->points[1]);
        tex2_swap(&triangle->tex_coords[0], &triangle->tex_coords[1]);
    }
    if (triangle->points[1].y > triangle->points[2].y) {
        vec4_swap(&triangle->points[1], &triangle->points[2]);
        tex2_swap(&triangle->tex_coords[1], &triangle->tex_coords[2]);
    }
    if (triangle->points[0].y > triangle->points[1].y) {
        vec4_swap(&triangle->points[0], &triangle->points[1]);
        tex2_swap(&triangle->tex_coords[0], &triangle->tex_coords[1]);
    }

    //Flip the V component for inverted UV coordinates
    triangle->tex_coords[0].v = 1.0 - triangle->tex_coords[0].v;    
    triangle->tex_coords[1].v = 1.0 - triangle->tex_coords[1].v;
    triangle->tex_coords[2].v = 1.0 - triangle->tex_coords[2].v;

    fill_textured_bottom_triangle(triangle, texture);

    fill_textured_top_triangle(triangle, texture);
}

// Render the upper part of the triangle (flat-bottom)
void fill_textured_bottom_triangle(triangle_t* triangle, const uint32_t* texture) {
    float left_inverse_slope = 0;
    float right_inverse_slope = 0;
        
    if (triangle->points[1].y - triangle->points[0].y != 0) left_inverse_slope = (triangle->points[1].x - triangle->points[0].x) / abs(triangle->points[1].y - triangle->points[0].y);
    if (triangle->points[2].y - triangle->points[0].y != 0) right_inverse_slope = (triangle->points[2].x - triangle->points[0].x) / abs(triangle->points[2].y - triangle->points[0].y);

    if (triangle->points[1].y - triangle->points[0].y != 0) {
        for (size_t y = triangle->points[0].y; y <= triangle->points[1].y; y++) {
            int x_start = triangle->points[1].x + (y - triangle->points[1].y) * left_inverse_slope;
            int x_end =   triangle->points[0].x + (y - triangle->points[0].y) * right_inverse_slope;

            if (x_end < x_start) {
                int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
            }

            for (size_t x = x_start; x < x_end; x++) {
                draw_texel(x, y, triangle, texture);
            }
        }
    }
}

// Render the bottom part of the triangle (flat-top)
void fill_textured_top_triangle(triangle_t* triangle, const uint32_t* texture) {
    float left_inverse_slope = 0;
    float right_inverse_slope = 0;

    if (triangle->points[2].y - triangle->points[1].y != 0) left_inverse_slope = (triangle->points[2].x - triangle->points[1].x) / abs(triangle->points[2].y - triangle->points[1].y);
    if (triangle->points[2].y - triangle->points[0].y != 0) right_inverse_slope = (triangle->points[2].x - triangle->points[0].x) / abs(triangle->points[2].y - triangle->points[0].y);

    if (triangle->points[2].y - triangle->points[1].y != 0) {
        for (int y = triangle->points[1].y; y <= triangle->points[2].y; y++) {
            int x_start = triangle->points[1].x + (y - triangle->points[1].y) * left_inverse_slope;
            int x_end = triangle->points[0].x + (y - triangle->points[0].y) * right_inverse_slope;

            if (x_end < x_start) {
                int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) {
                draw_texel(x, y, triangle, texture);
            }
        }
    }
}