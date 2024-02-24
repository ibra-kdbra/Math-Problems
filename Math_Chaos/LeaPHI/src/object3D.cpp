#include "object3D.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Object3D::Object3D() {
    m_position = glm::vec3(0.0f);
    m_orientation = glm::quat(glm::vec3(0.0f));
}

glm::mat4 Object3D::inverse_model_matrix() const {
    return glm::translate(glm::mat4_cast(m_orientation), -m_position);
}
glm::mat4 Object3D::model_matrix() const {
    return glm::affineInverse(this->inverse_model_matrix());
}

glm::vec3 Object3D::position() const {
    return m_position;
}
glm::quat Object3D::orientation() const {
    return m_orientation;
}

void Object3D::translate(glm::vec3 value) {
    m_position += value;
}
void Object3D::translate_x(float value) {
    this->translate(glm::vec3(value, 0.0f, 0.0f));
}
void Object3D::translate_y(float value) {
    this->translate(glm::vec3(0.0f, value, 0.0f));
}
void Object3D::translate_z(float value) {
    this->translate(glm::vec3(0.0f, 0.0f, value));
}

void Object3D::rotate(float angle, glm::vec3 axis) {
    glm::vec3 oaxis = axis * m_orientation;
    if (glm::length(oaxis) > 0.0f) {
        oaxis = glm::normalize(oaxis);
    }
    m_orientation *= glm::angleAxis(angle, oaxis);
}

void Object3D::orbit(float angle, glm::vec3 axis, glm::vec3 center) {
    glm::vec3 pc = center - this->position();
    this->translate(pc);

    glm::vec3 oaxis = axis * m_orientation;
    if (glm::length(oaxis) > 0.0f) {
        oaxis = glm::normalize(oaxis);
    }
    glm::quat aa = glm::angleAxis(angle, oaxis);
    m_orientation *= aa;

    glm::vec3 cp = -pc * aa;
    this->translate(cp);
}
