#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>

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
    auto start_time = std::chrono::high_resolution_clock::now();

    Eigen::MatrixXf xy = iter();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
    std::cout << "--- " << elapsed.count() << " seconds ---" << std::endl;

    // Plotting with SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Scatter Plot");
    sf::CircleShape point(0.5f);
    point.setFillColor(sf::Color::Black);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        window.clear(sf::Color::White);
        for (int i = 0; i < size; ++i) {
            point.setPosition(xy(0, i) * 100 + 400, xy(1, i) * 100 + 300);  // Scale and translate points
            window.draw(point);
        }
        window.display();
    }

    // Save the window content to a file
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    texture.update(window);
    texture.copyToImage().saveToFile("output.ppm");

    return 0;
}
