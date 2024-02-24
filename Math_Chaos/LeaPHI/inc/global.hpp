#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "window.hpp"

#define NAME "leaphi"
#define WIDTH 960
#define HEIGHT 540
#define ASPECT double(WIDTH) / HEIGHT
#define FOV 30.0f
#define ORBIT_CENTER glm::vec3(0.0f, 0.0f, 0.0f)

struct global_t {
    Window window = Window(NAME, WIDTH, HEIGHT);
    Camera camera = Camera(FOV, ASPECT);

    double delta;

    bool run_animation = false;
    float time = 0.0f;
    float growth_rate = 0.5f;
    float spawn_rate = 0.4f;

    struct mouse_t {
        bool LMB_down = false;
        bool MMB_down = false;
        bool RMB_down = false;
        glm::ivec2 last_pos;
    } mouse;
};

extern global_t global;
