#include "leaf.hpp"

#include <assert.h>

#include "bezier_curve.hpp"
#include "triangle_wrapper.hpp"
#include "util.hpp"

#define LEAF_ASPECT (2.0f / 3.0f)
#define LEAF_VARIANCE 12 // random-generation variance limiter {2...}
#define VEIN_STEM_RATIO (1.0f / (N_SAMPLES-1))

#define VEIN_C1_RADIUS_FACTOR 0.4f
#define VEIN_C2_RADIUS_FACTOR 0.8f
#define VEIN_C3_X_RADIUS_FACTOR 0.2f
#define VEIN_C3_Y_RADIUS_FACTOR (2.0f * VEIN_C3_X_RADIUS_FACTOR)
#define D_MIN_FACTOR 0.1f
#define D_MAX_FACTOR 0.5f
#define E_MIN_FACTOR 0.15f
#define E_MAX_FACTOR 1.0f
#define MARGIN_C2_OFFSET_FACTOR 0.6f

#define LEAF_COLOR glm::vec3(0.0f, 1.0f, 0.0f)
#define LEAF_SHADER Shader("leaf", "res/shaders/basic.vert", \
                                    "res/shaders/basic.frag")

struct sampled_leaf_t {
    samples_t stem;
    samples_t vein;
    samples_t u_margin;
    samples_t l_margin;
};

samples_t build_vein_samples(float f, float g, float size_factor) {
    float a = f * (M_PI / LEAF_VARIANCE) + (M_PI / 2);
    float b = -a + M_PI;

    float c0_x = 0.0f;
    float c0_y = 0.0f;

    float c1_radius = VEIN_C1_RADIUS_FACTOR * size_factor;
    float c1_x = c1_radius * sin(a);
    float c1_y = c1_radius * cos(a);

    float c2_radius = VEIN_C1_RADIUS_FACTOR * size_factor;
    float c2_x = c2_radius * sin(b);
    float c2_y = c2_radius * cos(b);

    float c3_x_radius = VEIN_C3_X_RADIUS_FACTOR * size_factor;
    float c3_y_radius = VEIN_C3_X_RADIUS_FACTOR * size_factor;
    float c3_x = c3_x_radius * sin(a) + c2_x;
    float c3_y = c3_y_radius * cos(a) + c2_y;

    Bezier_Curve vein;
    vein.add(glm::vec3(c0_x, c0_y, 0.0f));
    vein.add(glm::vec3(c1_x, c1_y, 0.0f));
    vein.add(glm::vec3(c2_x, c2_y, 0.0f));
    vein.add(glm::vec3(c3_x, c3_y, 0.0f));

    return vein.sample(g);
}
samples_t build_upper_margin_samples(float g, float x_size, float y_size) {
    float c0_x = 0.0f;
    float c0_y = 0.0f;

    float d_max = D_MAX_FACTOR * x_size;
    float d_min = D_MIN_FACTOR * x_size;
    float d = d_max - g * (d_max-d_min);
    float c1_x = d;
    float c1_y = y_size;

    float e_max = E_MAX_FACTOR * y_size;
    float e_min = E_MIN_FACTOR * y_size;
    float e = e_max - g * (e_max-e_min);
    float c2_offset = MARGIN_C2_OFFSET_FACTOR * x_size;
    float c2_x = c2_offset;
    float c2_y = e;

    float c3_x = x_size;
    float c3_y = 0.0f;

    Bezier_Curve upper_margin;
    upper_margin.add(glm::vec3(c0_x, c0_y, 0.0f));
    upper_margin.add(glm::vec3(c1_x, c1_y, 0.0f));
    upper_margin.add(glm::vec3(c2_x, c2_y, 0.0f));
    upper_margin.add(glm::vec3(c3_x, c3_y, 0.0f));

    return upper_margin.sample();
}

void normalize_vein(samples_t& vein, float x_size) {
    float vein_size = vein.back().x;
    for (auto& s : vein) {
        s.x = s.x/vein_size * x_size;
    }
}
void invert_samples_x(samples_t& samples) {
    for (auto& s : samples) {
        s.x = -s.x;
    }
}
void invert_samples_y(samples_t& samples) {
    for (auto& s : samples) {
        s.y = -s.y;
    }
}
void apply_vein_offset(const samples_t& vein, samples_t& margin) {
    assert(vein.size() == margin.size());

    for (unsigned i = 0; i < margin.size(); ++i) {
        margin.at(i).y += vein.at(i).y;
    }
}

sampled_leaf_t build_sampled_leaf(float f, float g, float size_factor) {
    if (g > 1.0f) {
        size_factor += g - 1.0f;
        g = 1.0f;
    }

    float x_size = g * size_factor;
    float y_size = x_size * g * LEAF_ASPECT;

    samples_t vein_samples = build_vein_samples(f, g, size_factor);
    normalize_vein(vein_samples, x_size);

    unsigned offset = ceil(vein_samples.size() * VEIN_STEM_RATIO);
    samples_t stem_samples(vein_samples.begin(), vein_samples.begin()+offset);
    invert_samples_x(stem_samples);
    invert_samples_y(stem_samples);

    samples_t u_margin_samples = build_upper_margin_samples(g, x_size, y_size);
    samples_t l_margin_samples = u_margin_samples;
    invert_samples_y(l_margin_samples);
    apply_vein_offset(vein_samples, u_margin_samples);
    apply_vein_offset(vein_samples, l_margin_samples);

    return {stem_samples, vein_samples, u_margin_samples, l_margin_samples};
}
samples_t merge_margins(const samples_t& u_margin, const samples_t& l_margin) {
    assert(all_close(u_margin.front(), l_margin.front()));
    assert(all_close(u_margin.back(), l_margin.back()));

    samples_t margin;

    for (unsigned i = 0; i < u_margin.size()-1; ++i) {
        margin.push_back(u_margin.at(i));
    }
    for (unsigned i = 0; i < l_margin.size()-1; ++i) {
        unsigned last_i = l_margin.size() - 1;
        margin.push_back(l_margin.at(last_i - i));
    }

    return margin;
}

Mesh build_empty_leaf() {
    return Mesh(Geometry(v_buffer_t(), e_buffer_t()), LEAF_SHADER);
}
Mesh build_contour(const sampled_leaf_t& sampled_leaf) {
    Mesh vein = Bezier_Curve::mesh_from_samples(sampled_leaf.vein);
    Mesh stem = Bezier_Curve::mesh_from_samples(sampled_leaf.stem);
    Mesh u_margin = Bezier_Curve::mesh_from_samples(sampled_leaf.u_margin);
    Mesh l_margin = Bezier_Curve::mesh_from_samples(sampled_leaf.l_margin);

    Mesh leaf = stem + vein + u_margin + l_margin;
    leaf.translate(-sampled_leaf.stem.back());

    return leaf;
}
Mesh build_lamina(const sampled_leaf_t& sampled_leaf, const glm::vec3& color) {
    samples_t u_margin_samples = sampled_leaf.u_margin;
    samples_t l_margin_samples = sampled_leaf.l_margin;
    samples_t margin_samples = merge_margins(u_margin_samples, l_margin_samples);

    v_buffer_t vertices = buffer_from_samples(margin_samples, color);
    e_buffer_t elements = triangulate_xy_plane(margin_samples);

    return Mesh(Geometry(vertices, elements), LEAF_SHADER);
}

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Leaf::Leaf(float f, float size_factor) : m_f(f), m_size_factor(size_factor)  {
    float r = noise_p(0.3f);
    float g = 0.4f + noise_p(0.2f);
    float b = noise_p(0.1f);
    m_color = glm::vec3(r, g, b);
}
Leaf::Leaf() : Leaf(noise_np(1.0f),  DEFAULT_LEAF_SIZE) {
}

Mesh Leaf::mesh(float growth_stage) const {
    if (growth_stage <= 0.0f) {
        return build_empty_leaf();
    }
    sampled_leaf_t sampled_leaf = build_sampled_leaf(m_f, growth_stage, m_size_factor);


    Mesh leaf(build_contour(sampled_leaf));
    leaf.add(build_lamina(sampled_leaf, m_color));

    return leaf;
}
