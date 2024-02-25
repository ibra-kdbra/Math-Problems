#include "shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <assert.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <utility> // Add this line

#include "util.hpp"

void read_source(const char* path, std::string& source) {
    assert(source.size() == 0);

    std::ifstream file(path);
    if (!file.is_open()) {
        std::stringstream ss;
        ss << "read_source() error for path \'" << path << "\'";
        throw std::runtime_error(ss.str());
    }

    std::stringstream ss;
    ss << file.rdbuf();
    source = ss.str();
}

GLuint compile_shader(const char* source, GLenum type) {
    GLuint shader_id;

    GL(shader_id = glCreateShader(type));
    GL(glShaderSource(shader_id, 1, &source, NULL));
    GL(glCompileShader(shader_id));

    int success;
    char info_log[512];
    GL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success));
    if (!success) {
        GL(glGetShaderInfoLog(shader_id, 512, NULL, info_log));
        std::string st = (type == GL_VERTEX_SHADER)? "vertex" : "fragment";
        std::stringstream ss;
        ss << st << " shader compiling error: \n" << info_log;
        throw std::runtime_error(ss.str());
    }

    return shader_id;
}

GLuint create_program(const std::string& v_source, const std::string& f_source) {
    GLuint program_id;

    GLuint v_shader_id = compile_shader(v_source.c_str(), GL_VERTEX_SHADER);
    GLuint f_shader_id = compile_shader(f_source.c_str(), GL_FRAGMENT_SHADER);

    GL(program_id = glCreateProgram());
    GL(glAttachShader(program_id, v_shader_id));
    GL(glAttachShader(program_id, f_shader_id));
    GL(glLinkProgram(program_id));

    int success;
    char info_log[512];
    GL(glGetProgramiv(program_id, GL_LINK_STATUS, &success));
    if (!success) {
        GL(glGetProgramInfoLog(program_id, 512, NULL, info_log));
        std::stringstream ss;
        ss << "rhader linking error: \n" << info_log;
        throw std::runtime_error(ss.str());
    }

    GL(glDeleteShader(v_shader_id));
    GL(glDeleteShader(f_shader_id));

    return program_id;
}

void destroy_program(GLuint program_id) {
    GL(glUseProgram(0));
    GL(glDeleteProgram(program_id));
}

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Shader::Shader(const char* name, const char* v_path, const char* f_path) : m_program_name(name) {
    read_source(v_path, m_v_source);
    read_source(f_path, m_f_source);

    m_program_id = create_program(m_v_source, m_f_source);
}

Shader::Shader(const Shader& other) {
    m_v_source = other.m_v_source;
    m_f_source = other.m_f_source;
    m_program_id = create_program(m_v_source, m_f_source);
    m_program_name = std::move(other.m_program_name);
    // cache not copied, because I'm not sure it would work
}
Shader& Shader::operator=(Shader other) {
    swap(*this, other);
    return *this;
}
Shader::Shader(Shader&& other) {
    m_v_source = std::move(other.m_v_source);
    m_f_source = std::move(other.m_f_source);
    m_program_id = std::exchange(other.m_program_id, 0);
    m_program_name = std::move(other.m_program_name);
    m_uniform_location_cache = std::move(other.m_uniform_location_cache);
}
Shader& Shader::operator=(Shader&& other) {
    if (this != &other) {
        destroy_program(m_program_id);

        m_v_source = std::move(other.m_v_source);
        m_f_source = std::move(other.m_f_source);
        m_program_id = std::exchange(other.m_program_id, 0);
        m_program_name = std::move(other.m_program_name);
        m_uniform_location_cache = std::move(other.m_uniform_location_cache);
    }

    return *this;
}
Shader::~Shader() {
    destroy_program(m_program_id);
}

void swap(Shader& first, Shader& second) {
    using std::swap; // enables ADL

    swap(first.m_v_source, second.m_v_source);
    swap(first.m_f_source, second.m_f_source);
    swap(first.m_program_id, second.m_program_id);
    swap(first.m_program_name, second.m_program_name);
    swap(first.m_uniform_location_cache, second.m_uniform_location_cache);
}

void Shader::use() const {
    GL(glUseProgram(m_program_id));
}

void Shader::uniform(const std::string& name, float value) const {
    this->use();
    GLint location = this->uniform_location(name);
    GL(glUniform1f(location, value));
}
void Shader::uniform(const std::string& name, const glm::vec2& value) const {
    this->use();
    GLint location = this->uniform_location(name);
    GL(glUniform2f(location, value.x, value.y));
}
void Shader::uniform(const std::string& name, const glm::vec3& value) const {
    this->use();
    GLint location = this->uniform_location(name);
    GL(glUniform3f(location, value.x, value.y, value.z));
}
void Shader::uniform(const std::string& name, const glm::vec4& value) const {
    this->use();
    GLint location = this->uniform_location(name);
    GL(glUniform4f(location, value.x, value.y, value.z, value.w));
}
void Shader::uniform(const std::string& name, int value) const {
    this->use();
    GLint location = this->uniform_location(name);
    GL(glUniform1i(location, value));
}
void Shader::uniform(const std::string& name, const glm::ivec2& value) const {
    this->use();
    GLint location = this->uniform_location(name);
    GL(glUniform2i(location, value.x, value.y));
}
void Shader::uniform(const std::string& name, const glm::ivec3& value) const {
    this->use();
    GLint location = this->uniform_location(name);
    GL(glUniform3i(location, value.x, value.y, value.z));
}
void Shader::uniform(const std::string& name, const glm::ivec4& value) const {
    this->use();
    GLint location = this->uniform_location(name);
    GL(glUniform4i(location, value.x, value.y, value.z, value.w));
}
void Shader::uniform(const std::string& name, const glm::mat4& value) const {
    this->use();
    GLint location = this->uniform_location(name);
    GL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}

// private interface +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

GLint Shader::uniform_location(const std::string& name) const {
    GLint location;

    // first check location cache
    auto it = m_uniform_location_cache.find(name);
    if (it != m_uniform_location_cache.end()) {
        return it->second;
    }

    // not found, query opengl and cache result
    GL(location = glGetUniformLocation(m_program_id, name.c_str()));
    if (location < 0) {
        std::stringstream ss;
        ss << "no uniform with name \'" << name << "\' found in shader \'";
        ss << m_program_name << "\'";
        throw std::runtime_error(ss.str());
    }
    m_uniform_location_cache[name] = location;

    return location;
}
