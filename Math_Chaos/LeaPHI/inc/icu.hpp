#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void icu_window_size(GLFWwindow* win, int w, int h);
void icu_key(GLFWwindow* win, int key, int scode, int action, int mods);
void icu_mouse_button(GLFWwindow* win, int button, int action, int mods);
void icu_cursor_pos(GLFWwindow* win, double x_pos, double y_pos);
void icu_scroll(GLFWwindow* win, double x_offset, double y_offset);

void setup_gui();
void process_gui();
void render_gui();
void cleanup_gui();
