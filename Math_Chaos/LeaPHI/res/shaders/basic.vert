#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;

uniform mat4 u_matrix;

out vec4 i_color;

void main() {
    i_color = vec4(a_color, 1.0);
    gl_Position = u_matrix * vec4(a_pos, 1.0);
}

