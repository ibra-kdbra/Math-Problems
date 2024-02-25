#include "color_cube.hpp"

const v_buffer_t cube_vertices = {
    // color cube vertices
    // position             // color
    -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, // 0 back  left  bottom
     1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 0.0f, // 1 back  right bottom
     1.0f,  1.0f, -1.0f,    1.0f, 1.0f, 0.0f, // 2 back  right up
    -1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f, // 3 back  left  up
    -1.0f, -1.0f,  1.0f,    0.0f, 0.0f, 1.0f, // 4 front left  bottom
     1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 1.0f, // 5 front right bottom
     1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f, // 6 front right up
    -1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 1.0f  // 7 front left  up
};
const e_buffer_t cube_elements = {
    // color cube indices
    4, 5, 6, 4, 6, 7, // front
    0, 2, 1, 0, 3, 2, // back
    0, 4, 7, 0, 7, 3, // left
    5, 1, 2, 6, 5, 2, // right
    3, 7, 2, 2, 7, 6, // top
    4, 0, 1, 4, 1, 5, // bottom
};

Color_Cube::Color_Cube() : Geometry(cube_vertices, cube_elements) {
}
