#pragma once 

#include <stdint.h>
#include "../lodepng/lodepng.h"

typedef struct {
    float u;
    float v;
} tex2_t;

extern unsigned int texture_width;
extern unsigned int texture_height;

extern unsigned char* png_texture;
extern uint32_t* mesh_texture;

void load_png_texture_data(char* filename);
