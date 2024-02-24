#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "geometry.hpp"
#include "mesh.hpp"
#include "shader.hpp"

#define N_SAMPLES 16

typedef std::vector<glm::vec3> control_points_t;
typedef std::vector<glm::vec3> samples_t;

class Bezier_Curve {
    public:
        Bezier_Curve(control_points_t points = control_points_t());

        void add(glm::vec3 point);

        samples_t sample(float p_limit=1.0f, unsigned n_samples=N_SAMPLES) const;

        static Mesh mesh_from_samples(const samples_t& samples);

    private:
        control_points_t m_points;
};
