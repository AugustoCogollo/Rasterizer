#include "ren_math.h"
#include <stdio.h>

vec3_t barycentric_weights(vec2_t* a, vec2_t* b, vec2_t* c, vec2_t* p) {
    vec2_t* ac = vec2_sub(c, a);
    vec2_t* ab = vec2_sub(b, a);
    vec2_t* ap = vec2_sub(p, a);
    vec2_t* pc = vec2_sub(c, p);
    vec2_t* pb = vec2_sub(b, p);
    float area_parallelogram_abc = (ac->x * ab->y - ac->y * ab->x);

    float alpha = area_parallelogram_abc != 0 ? (pc->x * pb->y - pc->y * pb->x) / area_parallelogram_abc : 0;
    float beta =  area_parallelogram_abc != 0 ? (ac->x * ap->y - ac->y * ap->x) / (area_parallelogram_abc) : 0;
    float gamma = 1 - alpha - beta;

    vec3_t weights = { alpha, beta, gamma };
    return weights;
}