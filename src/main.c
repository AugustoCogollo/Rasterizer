#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include "DArray/array.h"
#include "Display/display.h"
#include "Vectors/vector.h"
#include "Mesh/mesh.h"

triangle_t* triangles_to_render = NULL;
uint32_t mesh_color = 0xFFFF0000;

float fov_factor = 640;

bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0.0f;

vec3_t camera_position = { 0, 0, 0 };

void setup(void);
void process_input(void);
vec2_t project(vec3_t* point);
void update(void);
void render(void);
void free_resources(void);

int main(int argc, char* argv[]){
  is_running = initialize_window();

  setup();

  while(is_running){
    process_input();
    update();
    render();
  }

  clean_up();
  free_resources();

  return 0;
}


void setup(void) {
  color_buffer = malloc(sizeof(uint32_t) * window_width * window_height);
  assert(color_buffer && "Memory could not be allocated for the color buffer.\n");

  color_buffer_texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    window_width,
    window_height
  );

  //load_cube_mesh_data();
  load_obj_file("C:/msys64/home/augus/Rasterizer/assets/sphere.obj");
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type){
    case SDL_QUIT:
      is_running = false;
      break;
    
    case SDL_KEYDOWN:
      if(event.key.keysym.sym == SDLK_ESCAPE)
        is_running = false;
      break;
  }
}

vec2_t project(vec3_t* point) {
  vec2_t projected_point = {
    .x = (fov_factor * point->x) / point->z,
    .y = (fov_factor * point->y) / point->z
  };

  return projected_point;
}

void update(void) {
  uint32_t time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

  triangles_to_render = NULL;

  if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME){
    SDL_Delay(time_to_wait);
  }

  delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0f;
  previous_frame_time = SDL_GetTicks();

  mesh.rotation.x += 0.5 * delta_time;
  mesh.rotation.y += 0.5 * delta_time;
  mesh.rotation.z += 0.5 * delta_time;

  int num_faces = array_length(mesh.vertex_faces);
  for(size_t i = 0; i < num_faces; i++) {
    face_t mesh_face = mesh.vertex_faces[i];
    vec3_t face_vertices[3];
    face_vertices[0] = mesh.vertices[mesh_face.a - 1];
    face_vertices[1] = mesh.vertices[mesh_face.b - 1];
    face_vertices[2] = mesh.vertices[mesh_face.c - 1];

    triangle_t projected_triangle;

    vec3_t transformed_vertices[3];
    
    for(size_t j = 0; j < 3; j++) {
      vec3_t transformed_vertex = face_vertices[j];
      transformed_vertex = vec3_rotate_x(&transformed_vertex, mesh.rotation.x);
      transformed_vertex = vec3_rotate_y(&transformed_vertex, mesh.rotation.y);
      transformed_vertex = vec3_rotate_z(&transformed_vertex, mesh.rotation.z);

      //Translate the vertex away from the camera
      transformed_vertex.z += 5;
      transformed_vertices[j] = transformed_vertex;
    }

    //Check backface culling before projecting
    vec3_t vector_a = transformed_vertices[0];/*    A    */
    vec3_t vector_b = transformed_vertices[1];/*   / \   */
    vec3_t vector_c = transformed_vertices[2];/*  C---B  */

    vec3_t vector_ab = vec3_sub(&vector_b, &vector_a); //B-A vector
    vec3_normalize(&vector_ab);
    vec3_t vector_ac = vec3_sub(&vector_c, &vector_a); //C-A vector
    vec3_normalize(&vector_ac);

    //Compute face normal (left handed system)
    vec3_t normal = vec3_cross(&vector_ab, &vector_ac);

    //Normalize the face normal vector
    vec3_normalize(&normal);

    //Find the vector between the camera position and point A
    vec3_t camera_ray = vec3_sub(&camera_position, &vector_a);

    //Calculate how aligned the normal is with the camera ray 
    float dot_normal_camera = vec3_dot(&normal, &camera_ray);

    if(dot_normal_camera < 0) {
      continue;
    }

    for(size_t j = 0; j < 3; j++) {

      vec2_t projected_point = project(&transformed_vertices[j]);

      //Scale and transform the projected points to the middle of the screen
      projected_point.x += (window_width / 2);
      projected_point.y += (window_height / 2);

      projected_triangle.points[j] = projected_point;
    }

    array_push(triangles_to_render, projected_triangle);

  }
}

void render(void){
  //draw_grid(0xFFFFFFFF);

  //Render all the projected points
  int num_triangles = array_length(triangles_to_render);
  for(size_t i = 0; i < num_triangles; i++) {
    triangle_t triangle =  triangles_to_render[i];
    draw_rect(triangle.points[0].x, triangle.points[0].y, 4, 4, mesh_color);
    draw_rect(triangle.points[1].x, triangle.points[1].y, 4, 4, mesh_color);
    draw_rect(triangle.points[2].x, triangle.points[2].y, 4, 4, mesh_color);

    draw_triangle(triangle, mesh_color);
  }

  array_free(triangles_to_render);

  render_color_buffer();
  clear_color_buffer(0xFF000000);

  SDL_RenderPresent(renderer);
}

void free_resources(void) {
  array_free(mesh.vertices);
  array_free(mesh.tex_coords);
  array_free(mesh.normals);
  array_free(mesh.vertex_faces);
  array_free(mesh.texture_faces);
  array_free(mesh.normal_faces);
  free(color_buffer);
}