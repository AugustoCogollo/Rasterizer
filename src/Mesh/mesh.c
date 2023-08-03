#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mesh.h"
#include "../DArray/array.h"

size_t mesh_total_vertices = 0;
size_t mesh_total_tex_coords = 0;
size_t mesh_total_normals = 0;
size_t mesh_total_faces = 0;

mesh_t mesh = {
  .vertices = NULL,
  .normals = NULL,
  .faces = NULL,
  .translation = { 0, 0, 0 },
  .rotation = { 0, 0, 0 },
  .scale = { 1.0, 1.0, 1.0 }
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
  { .x = -1, .y = -1, .z = -1 }, //1
  { .x = -1, .y =  1, .z = -1 }, //2
  { .x =  1, .y =  1, .z = -1 }, //3
  { .x =  1, .y = -1, .z = -1 }, //4
  { .x =  1, .y =  1, .z =  1 }, //5
  { .x =  1, .y = -1, .z =  1 }, //6
  { .x = -1, .y =  1, .z =  1 }, //7
  { .x = -1, .y = -1, .z =  1 }  //8
};

face_t cube_faces[N_CUBE_FACES] = {
    //front
    { .a = 1, .b = 2, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 } },
    { .a = 1, .b = 3, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 } },
    //right
    { .a = 4, .b = 3, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 } },
    { .a = 4, .b = 5, .c = 6, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 } },
    //back
    { .a = 6, .b = 5, .c = 7, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 } },
    { .a = 6, .b = 7, .c = 8, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 } },
    //left
    { .a = 8, .b = 7, .c = 2, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 } },
    { .a = 8, .b = 2, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 } },
    //top
    { .a = 2, .b = 7, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFF7F7F7F },
    { .a = 2, .b = 5, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFF7F7F7F },
    //bottom
    { .a = 6, .b = 8, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFF00 },
    { .a = 6, .b = 1, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFF00 }
};

void load_cube_mesh_data(void) {
  for(size_t i = 0; i < N_CUBE_VERTICES; i++) {
    vec3_t cube_vertex = cube_vertices[i];
    array_push(mesh.vertices, cube_vertex);
  }
  for(size_t i = 0; i < N_CUBE_FACES; i++) {
    face_t cube_face = cube_faces[i];
    array_push(mesh.faces, cube_face);
  }
}

void prepare_mesh(char* filename) {
  FILE* file_id;
  char line[1024];

  if((file_id = fopen(filename, "r")) == NULL) {
    printf("There was an error trying to open the file \n");
    return;
  }

  while(fgets(line, sizeof(line), file_id)) {
    char* saveptr = NULL;
    char line_copy[1024];
    strcpy(line_copy, line);
    char* token = strtok_r(line_copy, " ", &saveptr);

    if(strcmp(token, "v") == 0)
      mesh_total_vertices++;

    if(strcmp(token, "vt") == 0)
      mesh_total_tex_coords++;

    if(strcmp(token, "vn") == 0)
      mesh_total_normals++;

    if(strcmp(token, "f") == 0)
      mesh_total_faces++;
  }
  mesh.vertices = malloc(sizeof(vec3_t) * mesh_total_vertices);
  mesh.normals = malloc(sizeof(vec3_t) * mesh_total_normals);
  mesh.faces = malloc(sizeof(face_t) * mesh_total_faces);
}

void load_obj_file(char* filename) {
  FILE* file_id;
  char line[1024];

  if((file_id = fopen(filename, "r")) == NULL) {
    printf("There was an error trying to open the file \n");
    return;
  }

  size_t current_vertex = 0;
  size_t current_tex_coord = 0;
  size_t current_normal = 0;
  size_t current_face = 0;

  tex2_t tex_coords[mesh_total_tex_coords];

  while(fgets(line, sizeof(line), file_id)) {
    char* saveptr = NULL;
    char line_copy[1024];
    strcpy(line_copy, line);
    char* token = strtok_r(line_copy, " ", &saveptr);

    //strncmp can also be used
    if(strcmp(token, "v") == 0){
      vec3_t vertex;
      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, & vertex.z);
      mesh.vertices[current_vertex] = vertex;
      current_vertex++;
    }

    else if(strcmp(token, "vt") == 0) {
      tex2_t tex_coord;
      sscanf(line, "vt %f %f", &tex_coord.u, &tex_coord.v);
      tex_coords[current_tex_coord] = tex_coord;
      current_tex_coord++;  
    }

    else if(strcmp(token, "vn") == 0){
      vec3_t normal;
      sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
      mesh.normals[current_normal] = normal;
      current_normal++;
    }

    else if(strcmp(token, "f") == 0) {
        char* saveptr_index = NULL;
        char* data;
        face_t face;

        int texture_indices[3];

        //This first assignment of data returns f so the second assignment gives us the index information like 1/1/1 and the second would return 2/2/1 inside cube.obj
        data = strtok_r(line, " ", &saveptr_index); 
        data = strtok_r(NULL, " ", &saveptr_index);
        sscanf(data, "%d/%d/%d", &face.a, &texture_indices[0], &face.a_norm);

        data = strtok_r(NULL, " ", &saveptr_index);
        sscanf(data, "%d/%d/%d", &face.b, &texture_indices[1], &face.b_norm);

        data = strtok_r(NULL, " ", &saveptr_index);
        sscanf(data, "%d/%d/%d", &face.c, &texture_indices[2], &face.c_norm);

        --face.a;
        --face.b;
        --face.c;
        face.a_uv = tex_coords[texture_indices[0] - 1];
        face.b_uv = tex_coords[texture_indices[1] - 1];
        face.c_uv = tex_coords[texture_indices[2] - 1];
        face.color = 0xFFFFFFFF;

        mesh.faces[current_face] = face;
        current_face++;
    }
    //free(tex_coords);
  }

  fclose(file_id);
}

void destroy_mesh(mesh_t* mesh) {
  free(mesh->vertices);
  free(mesh->normals);
  free(mesh->faces);
}