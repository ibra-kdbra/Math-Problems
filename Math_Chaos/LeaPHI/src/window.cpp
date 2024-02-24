#include "window.hpp"

#include <assert.h>
#include <sstream>
#include <stdexcept>

#include "icu.hpp"
#include "util.hpp"

#define CONTEXT_MAJOR_VERSION 3
#define CONTEXT_MINOR_VERSION 3
#define N_SAMPLES 1
#define V_SYNC_FLAG true

void error_callback(int error, const char* description) {
    std::stringstream ss;
    ss << "glfw_error_callback(): " << description;
    throw std::runtime_error(ss.str());
}

void set_callbacks(GLFWwindow* win) {
    glfwSetErrorCallback(error_callback);

    glfwSetWindowSizeCallback(win, icu_window_size);
    glfwSetKeyCallback(win, icu_key);
    glfwSetMouseButtonCallback(win, icu_mouse_button);
    glfwSetCursorPosCallback(win, icu_cursor_pos);
    glfwSetScrollCallback(win, icu_scroll);
}

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

int Window::m_n_windows = 0;

Window::Window(const char* title, unsigned width, unsigned height) {
    if (m_n_windows == 0 && glfwInit() == GLFW_FALSE) {
        throw std::runtime_error("Window() error - glfwInit() failed");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, N_SAMPLES);

    m_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (m_window == NULL) {
        throw std::runtime_error("Window() error - glfwCreateWindow() failed");
    }

    this->make_context_current();
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Window() error - glewInit() failed");
    }

    set_callbacks(m_window);
    this->set_vsync(V_SYNC_FLAG);

    assert(glfwGetError(NULL) == GLFW_NO_ERROR);
    m_last_update_time = glfwGetTime();
    m_n_windows++;
}
Window::~Window() {
    this->make_context_current();
    glfwDestroyWindow(m_window);

    m_n_windows--;

    if (m_n_windows == 0) {
        glfwTerminate();
    }
}

void Window::make_context_current() {
    glfwMakeContextCurrent(m_window);
}
void Window::set_vsync(bool flag) {
    m_vsync_flag = flag;
    glfwSwapInterval(m_vsync_flag? 1 : 0);
}
void Window::toggle_vsync() {
    this->set_vsync(m_vsync_flag? false : true);
}

bool Window::should_close() const {
    return glfwWindowShouldClose(m_window);
}
glm::ivec2 Window::size() const {
    int w, h;
    glfwGetWindowSize(m_window, &w, &h);
    return glm::ivec2(w, h);
}
double Window::aspect() const {
    glm::ivec2 size = this->size();
    return double(size.x) / size.y;
}
GLFWwindow* Window::get_GLFWwindow() const {
    return m_window;
}

double Window::update() {
    this->make_context_current();
    glfwSwapBuffers(m_window);
    glfwPollEvents();

    // compute millisecond delta since last call
    double curr_time = glfwGetTime();
    double delta = curr_time - m_last_update_time;
    m_last_update_time = curr_time;

    return delta;
}
