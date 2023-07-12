#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "mesh.h"
#include "../DArray/array.h"

mesh_t mesh = {
  .vertices = NULL,
  .tex_coords = NULL,
  .normals = NULL,
  .vertex_faces = NULL,
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
    { .a = 1, .b = 2, .c = 3, .color = 0xFFFF0000 },
    { .a = 1, .b = 3, .c = 4, .color = 0xFFFF0000 },
    //right
    { .a = 4, .b = 3, .c = 5, .color = 0xFF0000FF },
    { .a = 4, .b = 5, .c = 6, .color = 0xFF0000FF },
    //back
    { .a = 6, .b = 5, .c = 7, .color = 0xFF00FF00 },
    { .a = 6, .b = 7, .c = 8, .color = 0xFF00FF00 },
    //left
    { .a = 8, .b = 7, .c = 2, .color = 0xFF00FFFF },
    { .a = 8, .b = 2, .c = 1, .color = 0xFF00FFFF },
    //top
    { .a = 2, .b = 7, .c = 5, .color = 0xFF7F7F7F },
    { .a = 2, .b = 5, .c = 3, .color = 0xFF7F7F7F },
    //bottom
    { .a = 6, .b = 8, .c = 1, .color = 0xFFFFFF00 },
    { .a = 6, .b = 1, .c = 4, .color = 0xFFFFFF00 }
};

void load_cube_mesh_data(void) {
  for(size_t i = 0; i < N_CUBE_VERTICES; i++) {
    vec3_t cube_vertex = cube_vertices[i];
    array_push(mesh.vertices, cube_vertex);
  }
  for(size_t i = 0; i < N_CUBE_FACES; i++) {
    face_t cube_face = cube_faces[i];
    array_push(mesh.vertex_faces, cube_face);
  }
}

void load_obj_file(char* filename) {
  FILE* file_id;
  //int size = 1024, pos;
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
    //printf("Token: %s\n", token);

    //strncmp can also be used 
    if(strcmp(token, "v") == 0){
      vec3_t vertex;
      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, & vertex.z);
      array_push(mesh.vertices, vertex);
    }

    else if(strcmp(token, "vt") == 0){
      vec2_t tex_coord;
      sscanf(line, "vt %f %f", &tex_coord.x, &tex_coord.y);
      array_push(mesh.tex_coords, tex_coord);
    }

    else if(strcmp(token, "vn") == 0){
      vec3_t normal;
      sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
      array_push(mesh.normals, normal);
    }

    else if(strcmp(token, "f") == 0){
        char* saveptr_index = NULL;
        char* data;
        face_t vertex_face;
        face_t texture_face;
        face_t normal_face;
        
        //This first assignment of data returns f so the second assignment gives us the index information like 1/1/1 and the second would return 2/2/1 inside cube.obj
        data = strtok_r(line, " ", &saveptr_index);
        data = strtok_r(NULL, " ", &saveptr_index);
        sscanf(data, "%d/%d/%d", &vertex_face.a, &texture_face.a, &normal_face.a);

        data = strtok_r(NULL, " ", &saveptr_index);
        sscanf(data, "%d/%d/%d", &vertex_face.b, &texture_face.b, &normal_face.b);

        data = strtok_r(NULL, " ", &saveptr_index);
        sscanf(data, "%d/%d/%d", &vertex_face.c, &texture_face.b, &normal_face.c);

        array_push(mesh.vertex_faces, vertex_face);
        array_push(mesh.texture_faces, texture_face);
        array_push(mesh.normal_faces, normal_face);
    }
  }

  fclose(file_id);
}