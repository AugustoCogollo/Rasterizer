#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include "Display/display.h"
#include "Vectors/vector.h"

bool is_running = false;
#define N_POINTS (9 * 9 * 9)

vec3_t cube_points[N_POINTS]; 

void setup(void);
void process_input(void);
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

void process_input(void){
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

void update(void){

}

void render(void){
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  draw_grid(0x000000);
  draw_rect(300, 200, 300, 150, 0xFF0088);
  draw_pixel(30, 30, 0xFF0088);

  render_color_buffer();
  clear_color_buffer(0xFFFFFFFF);

  SDL_RenderPresent(renderer);
}