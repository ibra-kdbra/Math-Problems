#include "icu.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>

#include "global.hpp"
#include "util.hpp"

void apply_orbit_rotation(double x_pos, double y_pos) {
    glm::ivec2 win_size = global.window.size();

    glm::vec3 v0 = arc_ball_mapping(global.mouse.last_pos, win_size);
    glm::vec3 v1 = arc_ball_mapping(glm::vec2(x_pos, y_pos), win_size);

    double angle = acos(std::max(-1.0f, std::min(1.0f, glm::dot(v0, v1))));
    glm::vec3 axis = glm::cross(v1, v0);
    global.camera.orbit(-angle, axis, ORBIT_CENTER);
}

// used for drag offset, so shortest dist to perpendicular plane is computed
float compute_object_distance() {
    glm::vec3 pc = ORBIT_CENTER - global.camera.position();
    return glm::dot(global.camera.front(), pc);
}

// apply window space offset vector perspectively adjusted for object distance
void apply_drag_offset(float x_pos, float y_pos) {
    glm::vec2 win_size = global.window.size();
    float dist = compute_object_distance();

    glm::vec2 window_v0 = glm::vec2(global.mouse.last_pos) / win_size;
    glm::vec2 window_v1 = glm::vec2(x_pos, y_pos) / win_size;

    glm::vec3 world_v0 = global.camera.embed_in_world_space(window_v0, dist);
    glm::vec3 world_v1 = global.camera.embed_in_world_space(window_v1, dist);

    glm::vec3 world_offset = world_v0 - world_v1;

    global.camera.translate(world_offset);
}

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

void icu_window_size(GLFWwindow* win, int w, int h) {
    GL(glViewport(0, 0, w, h));
    global.camera.set_aspect(float(w)/h);
}

void icu_key(GLFWwindow* win, int key, int scode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(win, GLFW_TRUE);
                break;
            case GLFW_KEY_SPACE:
                global.run_animation = (global.run_animation)? false : true;
                break;
        };
    }
}

void icu_mouse_button(GLFWwindow* win, int button, int action, int mods) {
    if (mods == GLFW_MOD_SHIFT && button == GLFW_MOUSE_BUTTON_LEFT) {
        button = GLFW_MOUSE_BUTTON_MIDDLE;
    }

    if (action == GLFW_PRESS) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                global.mouse.LMB_down = true;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                global.mouse.MMB_down = true;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                global.mouse.RMB_down = true;
                break;
        }
    }

    if (action == GLFW_RELEASE) {
        global.mouse.LMB_down = false;
        global.mouse.MMB_down = false;
        global.mouse.RMB_down = false;
    }
}

void icu_cursor_pos(GLFWwindow* win, double x_pos, double y_pos) {
    if (global.mouse.LMB_down) {
        /* apply_orbit_rotation(x_pos, y_pos); */
    }

    if (global.mouse.MMB_down) {
        apply_drag_offset(x_pos, y_pos);
    }

    if (global.mouse.RMB_down) {
    }

    global.mouse.last_pos = glm::ivec2(x_pos, y_pos);
}

void icu_scroll(GLFWwindow* win, double x_offset, double y_offset) {
    float speed = 4.0f;
    global.camera.zoom(speed*-y_offset);
}

void gui_window() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::Begin("leaphi control", NULL, window_flags);

    ImGui::SliderFloat("growth time", &(global.time), 0.0f, 100.0f);
    ImGui::SliderFloat("growth rate", &(global.growth_rate), 0.0f, 2.0f);
    ImGui::SliderFloat("spawn rate", &(global.spawn_rate), 0.0f, 2.0f);

    ImGui::End();
}
void setup_gui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(global.window.get_GLFWwindow(), true);
    ImGui_ImplOpenGL3_Init("#version 150");

    ImGuiStyle& style = ImGui::GetStyle();
    style.GrabRounding = 0.0f;
    style.WindowRounding = 0.0f;
}
void process_gui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    gui_window();

}
void render_gui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void cleanup_gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
