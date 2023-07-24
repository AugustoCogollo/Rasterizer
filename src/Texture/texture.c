#include "texture.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int texture_width = 64;
unsigned int texture_height = 64;

unsigned char* png_texture;
uint32_t* mesh_texture = NULL;

void load_png_texture_data(char* filename)
{
    unsigned int error = lodepng_decode32_file(&png_texture, &texture_width, &texture_height, filename);
    if(error) 
        printf("error %u: %s\n", error, lodepng_error_text(error));

    mesh_texture = (uint32_t*) png_texture;
    free(png_texture);
}