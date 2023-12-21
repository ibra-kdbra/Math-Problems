// main.cpp
#include <Eigen/Dense>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "OrbitCamera.h"

// Initial conditions
const int size = 100000;
const float a = 0.9f, b = -0.6013f, c = 2.0f, d = 0.5f;
const float x_o = -0.72f, y_o = -0.64f;

float fnx(float x, float y) {
    return x*x - y*y + a*x + b*y;
}

float fny(float x, float y) {
    return 2*x*y + c*x + d*y;
}

Eigen::MatrixXf iter() {
    Eigen::MatrixXf pl(2, size);
    pl(0, 0) = x_o;
    pl(1, 0) = y_o;
    for (int i = 1; i < size; ++i) {
        pl(0, i) = fnx(pl(0, i-1), pl(1, i-1));
        pl(1, i) = fny(pl(0, i-1), pl(1, i-1));
    }
    return pl;
}

int main() {
    // Calculate xy
    Eigen::MatrixXf xy = iter();

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Scatter Plot", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // OrbitCamera camera(5.0f);

    double lastX = 400, lastY = 300;
    float yaw = -90.0f, pitch = 0.0f;
    float speed = 0.05f;
    Eigen::Vector3f cameraPos(0.0f, 0.0f, 3.0f);
    Eigen::Vector3f cameraFront(0.0f, 0.0f, -1.0f);
    Eigen::Vector3f cameraUp(0.0f, 1.0f, 0.0f);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Handle mouse input
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float xoffset = mouseX - lastX;
        float yoffset = lastY - mouseY; // reversed since y-coordinates range from bottom to top
        lastX = mouseX;
        lastY = mouseY;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        Eigen::Vector3f front;
        front[0] = cos(yaw) * cos(pitch);
        front[1] = sin(pitch);
        front[2] = sin(yaw) * cos(pitch);
        cameraFront = front.normalized();

        // Render here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw points
        glBegin(GL_POINTS);
        for (int i = 0; i < size; ++i) {
            float t = static_cast<float>(i) / (size - 1);  // Normalize i to [0, 1]
            glColor3f((1 - t), 0, t);  // Interpolate from red to blue
            glVertex3f(xy(0, i), xy(1, i), 0.0f);  // Scale and translate points
        }
        glEnd();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}