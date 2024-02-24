#include <geometry.hpp>

#include <assert.h>
#include <iostream>
#include <limits>
#include <utility>

#include "util.hpp"

#define INF std::numeric_limits<float>::infinity()
#define DEFAULT_AABB {INF, -INF, INF, -INF, INF, -INF}

bool is_aligned(GLuint v_size, GLuint e_size, GLenum type) {
    if (e_size == 0) {
        return true;
    }

    switch (type) {
        case GL_POINTS:
            return (v_size >= 1);
        case GL_LINE_LOOP:
            return (v_size >= 1) && (e_size >= 2);
        case GL_LINES:
            return (v_size >= 2) && (e_size % 2 == 0);
        case GL_TRIANGLES:
            return (v_size >= 3) && (e_size % 3 == 0);
        default:
            throw std::runtime_error("GL_TYPE not supported");
    }
}

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Geometry::Geometry(const v_buffer_t& v_buffer, const e_buffer_t& e_buffer,
        GLenum type) : m_v_buffer(v_buffer), m_e_buffer(e_buffer) {
    m_bounding_box = this->compute_bounding_box(m_v_buffer);
    m_gbi = this->create_geometry_buffer(m_v_buffer, m_e_buffer, type);
}

Geometry::Geometry(const Geometry& other) : Geometry(other.m_v_buffer, other.m_e_buffer, other.m_gbi.type) {
}
Geometry& Geometry::operator=(Geometry other) {
    swap(*this, other);
    return *this;
}
Geometry::Geometry(Geometry&& other) {
    m_bounding_box = std::move(other.m_bounding_box);
    m_gbi = std::exchange(other.m_gbi, {0, 0, 0});
    m_v_buffer = std::move(other.m_v_buffer);
    m_e_buffer = std::move(other.m_e_buffer);
}
Geometry& Geometry::operator=(Geometry&& other) {
    if (this != &other) {
        this->destroy_geometry_buffer(m_gbi);

        m_bounding_box = std::move(other.m_bounding_box);
        m_gbi = std::exchange(other.m_gbi, {0, 0, 0});
        m_v_buffer = std::move(other.m_v_buffer);
        m_e_buffer = std::move(other.m_e_buffer);
    }

    return *this;
}
Geometry::~Geometry() {
    this->destroy_geometry_buffer(m_gbi);
}

void swap(Geometry& first, Geometry& second) {
    using std::swap; // enables ADL

    swap(first.m_bounding_box, second.m_bounding_box);
    swap(first.m_gbi, second.m_gbi);
    swap(first.m_v_buffer, second.m_v_buffer);
    swap(first.m_e_buffer, second.m_e_buffer);
}
Geometry operator+(const Geometry& first, const Geometry& second) {
    if (first.m_gbi.type != second.m_gbi.type) {
        throw std::runtime_error("Geometry::operator+() for different gbi types");
    }

    if (first.m_e_buffer.size() == 0) {
        return Geometry(second);
    }

    const v_buffer_t& s_v_buffer = second.m_v_buffer;
    v_buffer_t v_combined = first.m_v_buffer;
    v_combined.insert(v_combined.end(), s_v_buffer.begin(), s_v_buffer.end());

    e_buffer_t e_combined = first.m_e_buffer;
    unsigned offset = first.m_v_buffer.size() / 6; // 3 float pos + 3 float color
    for (unsigned e : second.m_e_buffer) {
        e_combined.push_back(e + offset);
    }

    return Geometry(v_combined, e_combined, first.m_gbi.type);
}

v_buffer_t Geometry::get_vertices() const {
    return m_v_buffer;
}

void Geometry::draw() const {
    GL(glBindVertexArray(m_gbi.vao));
    GL(glDrawElements(m_gbi.type, m_gbi.n_elements, GL_UNSIGNED_INT, NULL));
    GL(glBindVertexArray(0));
}

// private interface +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Geometry::aabb_t Geometry::compute_bounding_box(const v_buffer_t& v_buffer) {
    const unsigned alignment = 3;
    assert(v_buffer.size() % alignment == 0);

    aabb_t bounding_box = DEFAULT_AABB;

    for (unsigned i = 0; i+alignment <= v_buffer.size(); i += alignment) {
        float v_x = v_buffer.at(i+0);
        float v_y = v_buffer.at(i+1);
        float v_z = v_buffer.at(i+2);

        if (v_x < m_bounding_box.x_min)
            bounding_box.x_min = v_x;
        if (v_x < m_bounding_box.x_max)
            bounding_box.x_max = v_x;

        if (v_y < m_bounding_box.y_min)
            bounding_box.y_min = v_y;
        if (v_y < m_bounding_box.y_max)
            bounding_box.y_max = v_y;

        if (v_z < m_bounding_box.z_min)
            bounding_box.z_min = v_z;
        if (v_z < m_bounding_box.z_max)
            bounding_box.z_max = v_z;
    }

    return bounding_box;
}

Geometry::gbi_t Geometry::create_geometry_buffer(const v_buffer_t& v_buffer,
                                                 const e_buffer_t& e_buffer,
                                                 GLenum type) {
    GLsizei v_size = sizeof(v_buffer[0]) * v_buffer.size();
    GLsizei e_size = sizeof(e_buffer[0]) * e_buffer.size();
    assert(is_aligned(v_size, e_size, type));

    GLuint vao;
    GL(glGenVertexArrays(1, &vao));
    GL(glBindVertexArray(vao));

    GLuint vbo;
    GL(glGenBuffers(1, &vbo));
    GL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL(glBufferData(GL_ARRAY_BUFFER, v_size, &v_buffer[0], GL_STATIC_DRAW));
    GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)0));
    GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)(3*sizeof(float))));
    GL(glEnableVertexAttribArray(0));
    GL(glEnableVertexAttribArray(1));

    GLuint ebo;
    GL(glGenBuffers(1, &ebo));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, e_size, &e_buffer[0], GL_STATIC_DRAW));

    GL(glBindVertexArray(0));
    GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    return {vao, vbo, ebo, (GLsizei)e_buffer.size(), type};
}
void Geometry::destroy_geometry_buffer(Geometry::gbi_t gbi) {
    GL(glBindVertexArray(0));
    GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    GL(glDeleteBuffers(1, &gbi.ebo));
    GL(glDeleteBuffers(1, &gbi.vbo));
    GL(glDeleteVertexArrays(1, &gbi.vao));
}
