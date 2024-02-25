#include "global.hpp"
#include <GL/glew.h>

#include <vector>

#include "color_cube.hpp"
#include "icu.hpp"
#include "leaf.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "util.hpp"

#define N_LEAFS 32
#define PHI float(0.618034f * M_PI)

typedef std::vector<Leaf> leaf_vector_t;

global_t global;

Mesh build_cube() {
    Color_Cube g;
    Shader s("base", "res/shaders/basic.vert", "res/shaders/basic.frag");
    Mesh mesh(g, s);
    return mesh;
}

void reset_opengl() {
    GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

    GL(glEnable(GL_DEPTH_TEST));

    GL(glEnable(GL_CULL_FACE));
    GL(glFrontFace(GL_CCW));
    GL(glCullFace(GL_BACK));

    GL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
}

void render(const Camera& cam, const Mesh& mesh) {
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    glm::mat4 p = cam.proj_matrix();
    glm::mat4 v = cam.view_matrix();
    glm::mat4 m = mesh.model_matrix();

    mesh.shader().uniform("u_matrix", p*v*m);

    mesh.draw();
}
void render(const Camera& cam, const leaf_vector_t& leafs) {
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    glm::mat4 pv = cam.proj_matrix() * cam.view_matrix();

    for (float i = leafs.size()-1; i >= 0; --i) {
        const Leaf& leaf = leafs.at(i);
        float g = global.growth_rate * (global.time - i * global.spawn_rate);
        if (g < 0.0f) {
            continue;
        }
        g = log10(g+1);

        Mesh mesh = leaf.mesh(g);
        glm::mat4 m = mesh.model_matrix();
        m = glm::rotate(glm::mat4(1.0f), i*PHI, glm::vec3(0.0f, 0.0f, 1.0f)) * m;
        mesh.shader().uniform("u_matrix", pv*m);

        mesh.draw();
        if (mesh.n_children() >= 1) {
            mesh.child(0).shader().uniform("u_matrix", pv*m);
            mesh.child(0).draw();
        }
    }
}

int main(void) {
    reset_opengl();
    setup_gui();

    leaf_vector_t leafs = leaf_vector_t(N_LEAFS);
    global.camera.translate(glm::vec3(0.0f, 0.0f, 4.0f));

    while (!global.window.should_close()) {
        global.delta = global.window.update();
        global.time += (global.run_animation)? global.delta : 0.0;
        process_gui();

        render(global.camera, leafs);
        render_gui();

        sleep(10);
    }

    cleanup_gui();

    return 0;
}
