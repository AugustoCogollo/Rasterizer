#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

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
    if(x > window_width || y > window_height || x < 0 || y < 0){
      //printf("A pixel could not be drawn at x: %d, y: %d", x, y);
      return;
    }

    color_buffer[(window_width * y) + x] = color;
}

void draw_rect(int x, int y, int width, int height, uint32_t color){
  for(size_t i = y; i < (height + y); i++)
    for(size_t j = x; j < (width + x); j++){
      draw_pixel(j, i, color);
    }
}

void draw_grid(uint32_t color){
  for(size_t y = 0; y < window_height; y++)
    for(size_t x = 0; x < window_width; x++){
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
  free(color_buffer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}