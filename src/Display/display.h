#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../Vectors/vector.h"
#include "../Triangle/triangle.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000/FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;

extern int window_width;
extern int window_height;

extern bool show_wireframe;
extern bool show_solid;
extern bool show_vertex;
extern bool enable_face_culling;
extern bool show_light;
extern bool show_textures;

bool initialize_window(void);
void draw_grid(uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_pixel(int x, int y, uint32_t color);
void draw_texel(int x, int y, triangle_t* triangle, uint32_t* texture);
void draw_line(vec2_t* point1, vec2_t* point2, uint32_t color);
void draw_triangle(triangle_t* triangle, uint32_t color);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void clean_up(void);