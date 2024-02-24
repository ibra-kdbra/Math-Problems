#pragma once

#include "mesh.hpp"

#define DEFAULT_LEAF_SIZE 1.0f
#define FULLY_GROWN 1.0f

class Leaf {
    public:
        // f: generation factor [-1,1] and size_factor: size of fully grown leaf
        Leaf(float f, float size_factor = DEFAULT_LEAF_SIZE);
        Leaf();

        // leaf mesh at growth stage g [0,inf] where g > 1.0f just scales it up
        Mesh mesh(float growth_stage = FULLY_GROWN) const;
    private:
        float m_f;
        float m_size_factor;
        glm::vec3 m_color;
};
