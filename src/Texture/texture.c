#include "texture.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int texture_width = 0;
unsigned int texture_height = 0;

uint32_t* mesh_texture = NULL;

void load_png_texture_data(const char* filename) {
  unsigned error;
  unsigned char* image = 0;

  error = lodepng_decode32_file(&image, &texture_width, &texture_height, filename);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

  mesh_texture = (uint32_t*) image;

  free(image);
}

void decodeTwoSteps(const char* filename) {
    unsigned error;
    unsigned char* image = 0;
    unsigned char* png = 0;
    size_t pngsize;
    printf("TW: %d | TH: %d", texture_width, texture_height);
    error = lodepng_load_file(&png, &pngsize, filename);
    if(!error) error = lodepng_decode32(&image, &texture_width, &texture_height, png, pngsize);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
    printf("TW: %d | TH: %d", texture_width, texture_height);

    free(png);

    mesh_texture = (uint32_t*) image;

    free(image);
}