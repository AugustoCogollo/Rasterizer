#pragma once

#include "../Vectors/vector.h"
#include "../Triangle/triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2)

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

typedef struct {
    vec3_t* vertices;
    face_t* vertex_faces;

    vec2_t* tex_coords;
    face_t* texture_faces;

    vec3_t* normals;
    face_t* normal_faces;
   
    vec3_t translation;
    vec3_t rotation;
    vec3_t scale;
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data(void);
void load_obj_file(char* filename);