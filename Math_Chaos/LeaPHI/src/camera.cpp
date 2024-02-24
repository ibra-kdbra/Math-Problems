#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#define MIN_FOV 2.0f
#define MAX_FOV 128.0f

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Camera::Camera(float fov_y, float aspect, float near, float far) : m_fov_y(fov_y), m_aspect(aspect), m_near(near), m_far(far) {
}

float Camera::fov_y() const {
    return m_fov_y;
}
float Camera::aspect() const {
    return m_aspect;
}
float Camera::near() const {
    return m_near;
}
float Camera::far() const {
    return m_far;
}

glm::mat4 Camera::view_matrix() const {
    return this->inverse_model_matrix();
}
glm::mat4 Camera::proj_matrix() const {
    return glm::perspective(glm::radians(m_fov_y), m_aspect, m_near, m_far);
}

glm::vec3 Camera::front() const {
    return glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f) * this->orientation());
}
glm::vec3 Camera::right() const {
    return glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f) * this->orientation());
}
glm::vec3 Camera::up() const {
    return glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) * this->orientation());
}

window_plane_t Camera::window_plane(float dist) const {
    float y_length = tan(glm::radians(m_fov_y) / 2) * dist;
    float x_length = y_length * this->aspect();

    glm::vec3 origin = this->position() + dist * this->front();
    glm::vec3 x_axis = x_length * this->right();
    glm::vec3 y_axis = y_length * this->up();

    return {origin, x_axis, y_axis};
}
glm::vec3 Camera::embed_in_world_space(glm::vec2 window_coord, float dist) const {
    window_plane_t plane = this->window_plane(dist);

    glm::vec3 world_coord = plane.origin;
    world_coord += 2.0f*(window_coord.x-0.5f) * plane.x_axis;
    world_coord -= 2.0f*(window_coord.y-0.5f) * plane.y_axis;

    return world_coord;
}

void Camera::move_forward(float value) {
    this->translate(value * this->front());
}
void Camera::move_backward(float value) {
    this->move_forward(-value);
}
void Camera::move_right(float value) {
    this->translate(value * this->right());
}
void Camera::move_left(float value) {
    this->move_right(-value);
}
void Camera::move_up(float value) {
    this->translate(value * this->up());
}
void Camera::move_down(float value) {
    this->move_up(-value);
}

void Camera::zoom(float value) {
    m_fov_y = std::max(MIN_FOV, std::min(MAX_FOV, m_fov_y+value));
}

void Camera::set_aspect(float aspect) {
    m_aspect = aspect;
}
