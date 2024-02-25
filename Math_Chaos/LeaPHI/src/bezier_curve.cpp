#include "bezier_curve.hpp"

#include "util.hpp"

#define CURVE_COLOR glm::vec3(0.0f, 0.0f, 0.0f)
#define CURVE_SHADER Shader("bezier_curve", "res/shaders/basic.vert", \
                                            "res/shaders/basic.frag")

e_buffer_t generate_curve_e_buffer(unsigned n_vertices) {
    e_buffer_t buffer;

    for (unsigned i = 1; i < n_vertices; ++i) {
        buffer.push_back(i-1);
        buffer.push_back(i);
    }

    return buffer;
}

glm::vec3 de_casteljau(const control_points_t& points, float u) {
    assert(u >= 0.0f && u <= 1.0f);

    int n_points = points.size();
    int n_nodes = (n_points*n_points + n_points) / 2;

    // 0th rank
    std::vector<glm::vec3> nodes(n_nodes - n_points);
    nodes.insert(nodes.begin(), points.begin(), points.end());
    int offset = n_points;

    // remaining ranks
    int n_ranks = n_points-1;
    for (int r = 1; r <= n_ranks; ++r) {
        for (int i = 0; i <= n_ranks-r; ++i) {
            int index = i + offset;
            int r_child = index - (n_points - r);
            int l_child = r_child - 1;
            nodes[index] = (1.0f-u) * nodes[l_child] + u * nodes[r_child];
        }
        offset += n_points - r;
    }

    return nodes.back();
}

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Bezier_Curve::Bezier_Curve(control_points_t points) : m_points(std::move(points)) {
}

void Bezier_Curve::add(glm::vec3 point) {
    m_points.push_back(point);
}

samples_t Bezier_Curve::sample(float p_limit, unsigned n_samples) const {
    assert(p_limit >= 0.0f && p_limit <= 1.0f);
    n_samples = std::max(2u, n_samples);
    samples_t samples;

    for (unsigned i = 0; i < n_samples; ++i) {
        float p = p_limit * float(i) / (n_samples-1);
        samples.push_back(de_casteljau(m_points, p));
    }

    return samples;
}

Mesh Bezier_Curve::mesh_from_samples(const samples_t& samples) {
    v_buffer_t v_buffer = buffer_from_samples(samples, CURVE_COLOR);
    e_buffer_t e_buffer = generate_curve_e_buffer(samples.size());

    return Mesh(Geometry(v_buffer, e_buffer, GL_LINES), CURVE_SHADER);
}
