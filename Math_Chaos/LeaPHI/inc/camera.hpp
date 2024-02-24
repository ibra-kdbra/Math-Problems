#pragma once

#include "object3D.hpp"

struct window_plane_t {
    glm::vec3 origin;
    glm::vec3 x_axis;
    glm::vec3 y_axis;
};

class Camera : public Object3D {
    public:
        // fov_y in degree, aspect as w over h
        Camera(float fov_y, float aspect, float near = 0.1f, float far = 1000.0f);

        float fov_y() const;
        float aspect() const;
        float near() const;
        float far() const;

        glm::mat4 view_matrix() const;
        glm::mat4 proj_matrix() const;

        // normalized orientation-corrected axis
        glm::vec3 front() const;
        glm::vec3 right() const;
        glm::vec3 up() const;

        // window space embedded in world space at distance from camera
        // axis vectors are of length half the plane's dimensions
        // origin lies at the center
        window_plane_t window_plane(float dist) const;

        // window coordinate expected to be in ranges [0.0, 1.0]
        // origin assumed in upper left corner, y assumed pointing down
        // dist - window plane distance from camera
        glm::vec3 embed_in_world_space(glm::vec2 window_coord, float dist) const;

        void move_forward(float value);
        void move_backward(float value);
        void move_right(float value);
        void move_left(float value);
        void move_up(float value);
        void move_down(float value);

        void zoom(float value);

        void set_aspect(float aspect);
    private:
        float m_fov_y;
        float m_aspect;
        float m_near;
        float m_far;
};
