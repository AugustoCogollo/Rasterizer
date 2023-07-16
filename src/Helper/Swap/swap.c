#include "swap.h"

void triangle_swap(triangle_t* a, triangle_t* b) {
    triangle_t temp = *a;
    *a = *b;
    *b = temp;
}

void int_swap(int* a, int* b) {
  int temp = *a;
  *a= *b;
  *b = temp;
}

void vec3_swap(vec3_t* a, vec3_t* b) {
    vec3_t temp = *a;
    *a = *b;
    *b = temp;
}

void vec2_swap(vec2_t* a, vec2_t* b) {
    vec2_t temp = *a;
    *a = *b;
    *b = temp;
}

void tex2_swap(tex2_t* a, tex2_t* b) {
    tex2_t temp = *a;
    *a = *b;
    *b = temp;
}