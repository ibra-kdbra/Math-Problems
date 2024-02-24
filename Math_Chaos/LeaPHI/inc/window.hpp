#pragma once

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
    public:
        Window(const char* title, unsigned width, unsigned height);
        ~Window();

        void make_context_current();
        void set_vsync(bool flag);
        void toggle_vsync();

        bool should_close() const;
        glm::ivec2 size() const;
        double aspect() const;
        GLFWwindow* get_GLFWwindow() const;

        double update();
    private:
        static int m_n_windows;

        GLFWwindow* m_window;
        bool m_vsync_flag;
        double m_last_update_time;
};
