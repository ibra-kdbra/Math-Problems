#pragma once

#include <glm/glm.hpp>

#include <stdio.h>
#include <string>
#include <vector>

#include "geometry.hpp"

#define DEBUG

#ifdef DEBUG
#define GL(x) x;                                                               \
              {                                                                \
                  int gl_error = glGetError();                                 \
                  if (gl_error != GL_NO_ERROR) {                               \
                      fprintf(stderr, "OpenGL error: %d near '%s:%d'\n",       \
                              gl_error, __FILE__, __LINE__);                   \
                      exit(gl_error);                                          \
                  }                                                            \
              }
#else
#define GL(x) x
#endif

std::string to_string(const glm::vec2& value);
std::string to_string(const glm::vec3& value);

void sleep(int ms);

glm::vec3 arc_ball_mapping(glm::vec2 screen_vec, glm::ivec2 screen_size);

float noise_p(float range);  // [0, range]
float noise_np(float range); // [-range, range]
bool noise_b();

v_buffer_t buffer_from_samples(std::vector<glm::vec3> vector, glm::vec3 color);

bool all_close(float a, float b);
bool all_close(const glm::vec3& a, const glm::vec3& b);
