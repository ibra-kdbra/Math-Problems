#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

#include <Eigen/Dense>

class OrbitCamera {
public:
    OrbitCamera(float radius);

    void rotate(float dx, float dy);
    Eigen::Matrix4f getViewMatrix() const;

private:
    float radius;
    float azimuth;
    float elevation;
};

#endif  // ORBIT_CAMERA_H
