#include <math.h>
#include <stdio.h>
#include "display.h"
#include "../Helper/Mathr/ren_math.h"
#include "../DArray/array.h"
#include "../Texture/texture.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

bool show_wireframe = true;
bool show_solid = false;
bool show_vertex = false;
bool enable_face_culling = true;
bool show_light = false;
bool show_textures = false;

bool initialize_window(void) {
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL. \n");
    return false;
  }

  //If you do not want the window to be fullscreen delete or comment the next four lines
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  window_width = display_mode.w;
  window_height = display_mode.h;

  //Create a SDL_Window
  window = SDL_CreateWindow(
    NULL, 
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED,
    window_width,
    window_height,
    SDL_WINDOW_BORDERLESS
    );

  if(!window){
    fprintf(stderr, "Error creating SDL window. \n");
    return false;
  }

  //Create SDL_Renderer
  renderer = SDL_CreateRenderer(window, -1, 0);
  if(!renderer){
    fprintf(stderr, "Error creating SDL renderer. \n");
    return false;
  }

  return true;
}

void draw_pixel(int x, int y, uint32_t color) {
    if(x > window_width || y > window_height || x <= 0 || y <= 0){
      //printf("A pixel could not be drawn at x: %d, y: %d", x, y);
      return;
    }
    color_buffer[(window_width * y) + x] = color;
}

void draw_texel(int x, int y, triangle_t* triangle, uint32_t* texture) {
  vec2_t point = {x, y};
  vec2_t point_a = vec2_from_vec4(&triangle->points[0]);
  vec2_t point_b = vec2_from_vec4(&triangle->points[1]);
  vec2_t point_c = vec2_from_vec4(&triangle->points[2]);
  vec3_t weights = barycentric_weights(&point_a, &point_b, &point_c, &point);

  float alpha = weights.x;
  float beta = weights.y;
  float gamma = weights.z;

  // Variables to store the interpolated values of U, V, and also 1/w for the current pixel
  float interpolated_u;
  float interpolated_v;
  float interpolated_reciprocal_w;

  // Perform the interpolation of all U/w and V/w values using barycentric weights and a factor of 1/w
  interpolated_u = (triangle->tex_coords[0].u / triangle->points[0].w) * alpha + (triangle->tex_coords[1].u / triangle->points[1].w) * beta + (triangle->tex_coords[2].u / triangle->points[2].w) * gamma;
  interpolated_v = (triangle->tex_coords[0].v / triangle->points[0].w) * alpha + (triangle->tex_coords[1].v / triangle->points[1].w) * beta + (triangle->tex_coords[2].v / triangle->points[2].w) * gamma;

  // Also interpolate the value of 1/w for the current pixel
  interpolated_reciprocal_w = (1 / triangle->points[0].w) * alpha + (1 / triangle->points[1].w) * beta + (1 / triangle->points[2].w) * gamma;

  // Now we can divide back both interpolated values by 1/w
  interpolated_u /= interpolated_reciprocal_w;
  interpolated_v /= interpolated_reciprocal_w;

  // Map the UV coordinate to the full texture width and height
  int tex_x = abs((int)(interpolated_u * texture_width)) % texture_width;
  int tex_y = abs((int)(interpolated_v * texture_height)) % texture_height;

  int tex_index = (texture_width * tex_y) + tex_x;
  draw_pixel(x, y, texture[tex_index]);
}

void draw_line(vec4_t* point1, vec4_t* point2, uint32_t color) {
  int delta_x = (point2->x - point1->x);
  int delta_y = (point2->y - point1->y);

  //Side length will check which side is the longest 
  int side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);
  float x_inc = delta_x / (float)side_length;
  float y_inc = delta_y / (float)side_length;

  float current_x = point1->x;
  float current_y = point1->y;

  for(size_t i = 0; i <= side_length; i++) {
    draw_pixel(roundf(current_x), roundf(current_y), color);
    current_x += x_inc;
    current_y += y_inc;
  }
}

void draw_triangle(triangle_t* triangle, uint32_t color) {
  draw_line(&triangle->points[0], &triangle->points[1], color);
  draw_line(&triangle->points[0], &triangle->points[2], color);
  draw_line(&triangle->points[1], &triangle->points[2], color);
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
  for(size_t i = y; i < (height + y); i++)
    for(size_t j = x; j < (width + x); j++) {
      draw_pixel(j, i, color);
    }
}

void draw_grid(uint32_t color) {
  for(size_t y = 0; y < window_height; y++)
    for(size_t x = 0; x < window_width; x++) {
      if(y % 10 == 0 || x % 10 == 0) {
        color_buffer[(window_width * y) + x] = color;
      }
    }
}

void render_color_buffer(void){
  SDL_UpdateTexture(
    color_buffer_texture,
    NULL,
    color_buffer,
    (int)(window_width * sizeof(uint32_t))
  );

  SDL_RenderCopy(
      renderer, 
      color_buffer_texture, 
      NULL, 
      NULL
    );
}

void clear_color_buffer(uint32_t color){
  for(size_t y = 0; y < window_height; y++)
    for(size_t x = 0; x < window_width; x++){
      color_buffer[(window_width * y) + x] = color; 
    }
}

void clean_up(void){
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}