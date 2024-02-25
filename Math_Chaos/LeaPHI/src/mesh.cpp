#include "mesh.hpp"

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Mesh::Mesh(Geometry geometry, const Shader& shader) : m_geometry(std::move(geometry)), m_shader(std::move(shader)) {
}

Mesh operator+(const Mesh& first, const Mesh& second) {
    return Mesh(first.m_geometry + second.m_geometry, first.m_shader);
}

const Shader& Mesh::shader() const {
    return m_shader;
}

v_buffer_t Mesh::get_vertices() const {
    return m_geometry.get_vertices();
}

unsigned Mesh::n_children() const {
    return m_children.size();
}
Mesh& Mesh::child(unsigned i) {
    return m_children.at(i);
}
void Mesh::add(const Mesh& new_child) {
    m_children.push_back(new_child);
}

void Mesh::draw() const {
    m_shader.use();
    m_geometry.draw();
}
