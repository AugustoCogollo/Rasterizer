#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include "Display/display.h"
#include "Vectors/vector.h"

#define N_POINTS (9 * 9 * 9)
float fov_factor = 640;

bool is_running = false;

vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_position = {.x = 0, .y = 0, .z = 5};

void setup(void);
void process_input(void);
vec2_t project(vec3_t* point);
void update(void);
void render(void);

int main(int argc, char* argv[]){
  is_running = initialize_window();

  setup();


  
  while(is_running){
    process_input();
    update();
    render();
  }

  clean_up();

  return 0;
}


void setup(void) {
  color_buffer = malloc(sizeof(uint32_t) * window_width * window_height);
  assert(color_buffer && "Memory could not be allocated for the color buffer.\n");

  color_buffer_texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    window_width,
    window_height
  );

  size_t point_count = 0;

  for(float x = -1; x <= 1; x += 0.25)
    for(float y = -1; y <= 1; y += 0.25)
      for(float z = -1; z <= 1; z += 0.25) {
        vec3_t new_point = { .x = x, .y = y, .z = z };

        //This will first add the new_point to the cube_points array and after it is done point_count will be incremented by one
        cube_points[point_count++] = new_point;
      }

}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type){
    case SDL_QUIT:
      is_running = false;
      break;
    
    case SDL_KEYDOWN:
      if(event.key.keysym.sym == SDLK_ESCAPE)
        is_running = false;
      break;
  }
}

vec2_t project(vec3_t* point) {
  vec2_t projected_point = {
    .x = (fov_factor * point->x) / point->z,
    .y = (fov_factor * point->y) / point->z
  };

  return projected_point;
}

void update(void){
  for(size_t i = 0; i < N_POINTS; i++){
    vec3_t point = cube_points[i];

    //Move the points away from the camera
    point.z -= camera_position.z;

    vec2_t projected_point = project(&point);
    projected_points[i] = projected_point;
  }
}

void render(void){
  //draw_grid(0xFFFFFFFF);

  //Render all the projected points
  for(size_t i = 0; i < N_POINTS; i++){
    vec2_t projected_point = projected_points[i];
    draw_rect(
      projected_point.x + (window_width / 2), 
      projected_point.y + (window_height / 2), 
      4, 
      4, 
      0xFFFF0088
      );
  }

  render_color_buffer();
  clear_color_buffer(0xFF000000);

  SDL_RenderPresent(renderer);
}