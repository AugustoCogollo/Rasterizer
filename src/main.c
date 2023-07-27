#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include "DArray/array.h"
#include "Display/display.h"
#include "Vectors/vector.h"
#include "Mesh/mesh.h"
#include "Settings/settings.h"
#include "Colors/color.h"
#include "Matrix/matrix.h"
#include "Light/light.h"
#include "Triangle/triangle.h"
#include "Texture/texture.h"
#include "lodepng/lodepng.h"

#define PI 3.14159265358979323846

triangle_t* triangles_to_render = NULL;

bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0.0f;

vec3_t camera_position = { 0, 0, 0 };
mat4_t projection_matrix;

void setup(void);
void process_input(void);
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
  z_buffer = malloc(sizeof(float) * window_width * window_height);
  assert(color_buffer && "Memory could not be allocated for the color buffer.\n");

  color_buffer_texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_RGBA32,
    SDL_TEXTUREACCESS_STREAMING,
    window_width,
    window_height
  );

  //Initialize the perspective matrix
  float fov = PI / 3;
  float aspect = (float)window_height / (float)window_width;
  float znear = 0.1;
  float zfar = 100;
  projection_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

  //load_cube_mesh_data();
  load_obj_file("./assets/f22.obj");
  decodeTwoSteps("./assets/f22.png");

  vec3_normalize(&global_light.direction);
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type) {
    case SDL_QUIT:
      is_running = false;
      break;
    
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym) {
        case SDLK_ESCAPE:
          is_running = false;
          break;

        case SDLK_1:
          show_vertex = !show_vertex;
          break;

        case SDLK_2:
          show_wireframe = !show_wireframe;
          break;

        case SDLK_3:
          show_solid = !show_solid;
          show_textures = false;
          break;

        case SDLK_4:
          show_light = !show_light;
          break;

        case SDLK_5:
          show_textures = !show_textures;
          show_solid = false;
          break;

        case SDLK_c:
          enable_face_culling = !enable_face_culling;
          break;
      }
      
      break;
  }
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
  //mesh.rotation.y += 0.5 * delta_time;
  //mesh.rotation.z += 0.5 * delta_time;

  // mesh.scale.x += 0.2 * delta_time;
  // mesh.scale.y += 0.2 * delta_time;
  // mesh.scale.z += 0.2 * delta_time;

  // mesh.translation.x += 0.5 * delta_time;
  mesh.translation.z = 5.0;  

  mat4_t world_matrix = mat4_identity();

  mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
  world_matrix = mat4_mult_mat4(&scale_matrix, &world_matrix);

  mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);
  world_matrix = mat4_mult_mat4(&rotation_matrix_z, &world_matrix);
  mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
  world_matrix = mat4_mult_mat4(&rotation_matrix_y, &world_matrix);
  mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
  world_matrix = mat4_mult_mat4(&rotation_matrix_x, &world_matrix);

  mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
  world_matrix = mat4_mult_mat4(&translation_matrix, &world_matrix);

  int num_faces = array_length(mesh.faces);
  for(size_t i = 0; i < num_faces; i++) {
    face_t mesh_face = mesh.faces[i];
    vec3_t face_vertices[3];
    face_vertices[0] = mesh.vertices[mesh_face.a];
    face_vertices[1] = mesh.vertices[mesh_face.b];
    face_vertices[2] = mesh.vertices[mesh_face.c];
    
    vec4_t transformed_vertices[3];
    
    for(size_t j = 0; j < 3; j++) {
      vec4_t transformed_vertex = vec4_from_vec3(&face_vertices[j]);

      transformed_vertex = mat4_mult_vec4(&world_matrix, &transformed_vertex);

      //Translate the vertex away from the camera
      transformed_vertices[j] = transformed_vertex;
    }

    uint32_t triangle_color = mesh_face.color;

    if(enable_face_culling) {
      //Check backface culling before projecting
      vec3_t vector_a = vec3_from_vec4(&transformed_vertices[0]);/*    A    */
      vec3_t vector_b = vec3_from_vec4(&transformed_vertices[1]);/*   / \   */
      vec3_t vector_c = vec3_from_vec4(&transformed_vertices[2]);/*  C---B  */

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

      if(show_light) {
        //Calculate how aligned the normal is with the camera ray
        float dot_normal_light = -vec3_dot(&normal, &global_light.direction);
        triangle_color = light_apply_intensity(triangle_color, dot_normal_light);
      }

      if(dot_normal_camera < 0) {
        continue;
      }
    }

    vec4_t projected_points[3];

    for(size_t j = 0; j < 3; j++) {
      projected_points[j] = mat4_mul_vec4_project(&projection_matrix, &transformed_vertices[j]);

      //Scale the projected points 
      projected_points[j].x *= window_width / 2.0;
      projected_points[j].y *= window_height / 2.0;

      //Invert the y values because of the y flipped screen coordinates
      projected_points[j].y *= -1;

      //Translate the projected points to the middle of the screen
      projected_points[j].x += (window_width / 2.0);
      projected_points[j].y += (window_height / 2.0);
    }

    //Calculate the average depth of the triangle vertices after transformation
    float depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z);

    triangle_t projected_triangle = {
      .points = {
        { projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w },
        { projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w },
        { projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w }
      },
      .tex_coords = {
        [0] = mesh_face.a_uv,
        [1] = mesh_face.b_uv,
        [2] = mesh_face.c_uv
      },
      .color = triangle_color,
      .avg_depth = depth
    };

    array_push(triangles_to_render, projected_triangle);
  }
}

void render(void) {
  //Render all the projected points
  int num_triangles = array_length(triangles_to_render);
  for(size_t i = 0; i < num_triangles; i++) {
    triangle_t triangle =  triangles_to_render[i];

    //Triangle points are rounded so that the model does not have gaps and black lines
    for(size_t i = 0; i < 3; i++) {
        triangle.points[i].x = roundf(triangle.points[i].x);
        triangle.points[i].y = roundf(triangle.points[i].y);
    }

    if(show_textures) {
      draw_textured_triangle(&triangle, mesh_texture);
    }

    if(show_solid) {
      draw_filled_triangle(&triangle);
    }

    if(show_wireframe) {
      draw_triangle(&triangle, WHITE);
    }

    if(show_vertex) {
      draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, YELLOW);
      draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, YELLOW);
      draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, YELLOW);
    }
  }
  
  array_free(triangles_to_render);

  render_color_buffer();
  clear_color_buffer(DARK_SLATE_GRAY);
  clear_z_buffer();

  SDL_RenderPresent(renderer);
}

void free_resources(void) {
  destroy_mesh(&mesh);
  free(color_buffer);
  free(z_buffer);
}