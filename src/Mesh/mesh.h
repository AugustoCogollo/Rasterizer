#pragma once

#include "../Vectors/vector.h"
#include "../Triangle/triangle.h"
#include "../Texture/texture.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2)

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

extern size_t mesh_total_vertices;
extern size_t mesh_total_tex_coords;
extern size_t mesh_total_normals;
extern size_t mesh_total_faces;

typedef struct {
    vec3_t* vertices;
    vec3_t* normals;

    face_t* faces;
   
    vec3_t translation;
    vec3_t rotation;
    vec3_t scale;
} mesh_t;

extern mesh_t mesh;

void prepare_mesh(char* filename);
void load_cube_mesh_data(void);
void load_obj_file(char* filename);
void destroy_mesh(mesh_t* mesh);