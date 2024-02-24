#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <map>
#include <string>

class Shader {
    public:
        Shader(const char* name, const char* v_path, const char* f_path);

        Shader(const Shader& other);
        Shader& operator=(Shader other);
        Shader(Shader&& other);
        Shader& operator=(Shader&& other);
        ~Shader();

        friend void swap(Shader& first, Shader& second);

        void use() const;

        void uniform(const std::string& name, float value) const;
        void uniform(const std::string& name, const glm::vec2& value) const;
        void uniform(const std::string& name, const glm::vec3& value) const;
        void uniform(const std::string& name, const glm::vec4& value) const;
        void uniform(const std::string& name, int value) const;
        void uniform(const std::string& name, const glm::ivec2& value) const;
        void uniform(const std::string& name, const glm::ivec3& value) const;
        void uniform(const std::string& name, const glm::ivec4& value) const;
        void uniform(const std::string& name, const glm::mat4& value) const;
    private:
        std::string m_v_source;
        std::string m_f_source;
        GLuint m_program_id;
        std::string m_program_name;
        mutable std::map<std::string, GLuint> m_uniform_location_cache;

        GLint uniform_location(const std::string& name) const;
};
