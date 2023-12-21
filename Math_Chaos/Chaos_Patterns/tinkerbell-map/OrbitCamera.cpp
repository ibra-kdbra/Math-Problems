// OrbitCamera.cpp
#include "OrbitCamera.h"

OrbitCamera::OrbitCamera(float radius)
    : radius(radius), azimuth(0.0f), elevation(0.0f) {}

void OrbitCamera::rotate(float dx, float dy) {
    azimuth += dx;
    elevation = std::clamp(elevation + dy, -89.0f, 89.0f);
}

Eigen::Matrix4f lookAt(const Eigen::Vector3f& eye, const Eigen::Vector3f& center, const Eigen::Vector3f& up) {
    Eigen::Vector3f f = (center - eye).normalized();
    Eigen::Vector3f u = up.normalized();
    Eigen::Vector3f s = f.cross(u).normalized();
    u = s.cross(f);

    Eigen::Matrix4f result = Eigen::Matrix4f::Identity();
    result << s.x(), s.y(), s.z(), -s.dot(eye),
               u.x(), u.y(), u.z(), -u.dot(eye),
              -f.x(), -f.y(), -f.z(),  f.dot(eye),
                0.0f,  0.0f,  0.0f,         1.0f;
    return result;
}

Eigen::Matrix4f OrbitCamera::getViewMatrix() const {
    Eigen::Vector3f eye(
        radius * std::cos(azimuth) * std::cos(elevation),
        radius * std::sin(elevation),
        radius * std::sin(azimuth) * std::cos(elevation)
    );
    Eigen::Vector3f up(0.0f, 1.0f, 0.0f);
    return lookAt(eye, Eigen::Vector3f::Zero(), up);
}
