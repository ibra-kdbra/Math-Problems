#pragma once

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Object3D {
    public:
        Object3D();

        glm::mat4 inverse_model_matrix() const;
        glm::mat4 model_matrix() const;

        glm::vec3 position() const;
        glm::quat orientation() const;

        void translate(glm::vec3 value);
        void translate_x(float value);
        void translate_y(float value);
        void translate_z(float value);

        void rotate(float angle, glm::vec3 axis);

        void orbit(float angle, glm::vec3 axis, glm::vec3 center);
    private:
        glm::vec3 m_position;
        glm::quat m_orientation;
};
