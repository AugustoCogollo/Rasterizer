#pragma once 

#include "../Vectors/vector.h"
#include "../Colors/color.h"
#include <stdint.h>

typedef struct {
    vec3_t direction;
} light_t;

extern light_t global_light;

uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor);