#include "util.hpp"

#include <assert.h>
#include <chrono>
#include <limits>
#include <sstream>
#include <thread>

std::string to_string(const glm::vec2& value) {
    std::stringstream ss;
    ss << "vec3(" << value.x << ", " << value.y << ")";
    return ss.str();
}
std::string to_string(const glm::vec3& value) {
    std::stringstream ss;
    ss << "vec3(" << value.x << ", " << value.y << ", " << value.z << ")";
    return ss.str();
}

void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

glm::vec3 arc_ball_mapping(glm::vec2 screen_vec, glm::ivec2 screen_size) {
    // Ken Shoemake's ArcBall sphere mapping
    glm::vec2 circle_center(screen_size.x/2, screen_size.y/2);
    double radius = screen_size.y/2;

    double x =  (screen_vec.x - circle_center.x) / radius;
    double y = -(screen_vec.y - circle_center.y) / radius;
    double z = 0.0;
    double r = x*x + y*y;

    if (1.0 - r <= 0.0) { // modified condition fixes rounding issue for sqrt
        assert(r > 1.0);
        double s = 1.0 / std::sqrt(r);
        x = s * x;
        y = s * y;
    } else {
        assert(1.0 - r > 0.0); // should be redundant due to modified condition
        z = std::sqrt(1.0 - r);
    }

    return glm::vec3(x, y, z);
}

float noise_p(float range) {
    return range * float(rand()) / float(RAND_MAX);
}
float noise_np(float range) {
    return 2.0f * noise_p(range) - range;
}
bool noise_b() {
    return (noise_p(1.0f) > 0.5f)? true : false;
}

v_buffer_t buffer_from_samples(std::vector<glm::vec3> vector, glm::vec3 color) {
    v_buffer_t buffer;

    for (const glm::vec3& vertex : vector) {
        buffer.push_back(vertex.x);
        buffer.push_back(vertex.y);
        buffer.push_back(vertex.z);
        buffer.push_back(color.r);
        buffer.push_back(color.g);
        buffer.push_back(color.b);
    }

    return buffer;
}

bool all_close(float a, float b) {
    float epsilon = std::numeric_limits<float>::epsilon();
    return a <= (b+epsilon) && a >= (b-epsilon);
}
bool all_close(const glm::vec3& a, const glm::vec3& b) {
    return all_close(a.x, b.x) && all_close(a.y, b.y) && all_close(a.z, b.z);
}
