# Barnsley Fern

The **Barnsley fern** is a **fractal** named after the British mathematician **Michael Barnsley** who first described it in his book *Fractals Everywhere*. He made it to resemble the black spleenwort, *Asplenium adiantum-nigrum*.

## Construction

The Barnsley fern is created by iterating over a large number of times on four mathematical equations, introduced by Barnsley, known as **Iterated Function System (IFS)**. These equations are **affine transformations** that map a point `(x, y)` to a new point `(x', y')` using a matrix multiplication and a translation. The equations are:

- `f1(x, y)`:

    $$\begin{bmatrix}0 \\ 0.16y\end{bmatrix}$$

- `f2(x, y)`:

    $$\begin{bmatrix}0.85x + 0.04y \\ -0.04x + 0.85y + 1.6\end{bmatrix}$$

- `f3(x, y)`:

    $$\begin{bmatrix}0.2x - 0.26y \\ 0.23x + 0.22y + 1.6\end{bmatrix}$$

- `f4(x, y)`:

    $$\begin{bmatrix}-0.15x + 0.28y \\ 0.26x + 0.24y + 0.44\end{bmatrix}$$

Each equation has a different probability of being chosen at each iteration, which affects the shape of the fern. The probabilities are:

- `f1`: 0.01
- `f2`: 0.85
- `f3`: 0.07
- `f4`: 0.07

The tip of the fern is the fixed point of `f1`, and the tips of the lowest two branches are the images of the main tip under `f3` and `f4`, respectively.

## Visualization

To visualize the Barnsley fern, we can use a **chaos game** algorithm, which starts with an initial point `(x0, y0)` and repeatedly applies one of the four equations randomly, according to their probabilities, to generate a new point `(x1, y1)`. Then, the new point becomes the input for the next iteration, and so on. The points are plotted on a plane as they are generated, forming the shape of the fern.

Here is an example of a C++ code that implements the chaos game for the Barnsley fern:

```cpp
// Include the header file for file operations
#include <fstream>

// Constants for the image dimensions and initial point location
const int height = 10000; // Height of the generated image
const int width = 5000 * 1.5; // Width of the generated image
bool graph[height][width] = { 0 }; // Initialize 2D array to represent the image, initially set all values to 0

// Define a structure for storing points
struct point {
    double x; // X-coordinate of the point
    double y; // Y-coordinate of the point

    // Constructor to initialize a point with specific values
    point(double x, double y) {
        this->x = x;
        this->y = y;
    }

    // Default constructor to initialize a point to (0, 0)
    point() {
        this->x = 0;
        this->y = 0;
    }
};

// Function to plot a point on the image
void plot(const point& p) {
    // Convert coordinates to integer indices within the image array
    int x = (int) ((p.x * 1000) + 2182) * 1.5; // Scale and offset x-coordinate
    int y = abs((int) ((p.y * 1000) - 10000)); // Scale and absolute value of y-coordinate

    // Check if the calculated coordinates are within the image bounds
    if (x >= 0 && x < width && y >= 0 && y < height) {
        // Mark the pixel at the calculated coordinates as part of the fern
        graph[y][x] = 1;
    }
}

// Define four functions f1, f2, f3, and f4 for the Barnsley fern
point f1(const point& p) {
    point res;
    res.x = 0;
    res.y = 0.16 * p.y; // Apply transformation for f1
    return res;
}

point f2(const point& p) {
    point res;
    res.x = (0.85 * p.x) + (0.04 * p.y);
    res.y = (-0.04 * p.x) + (0.85 * p.y) + 1.6; // Apply transformation for f2
    return res;
}

point f3(const point& p) {
    point res;
    res.x = (0.2 * p.x) - (0.26 * p.y);
    res.y = (0.23 * p.x) + (0.22 * p.y) + 1.6; // Apply transformation for f3
    return res;
}

point f4(const point& p) {
    point res;
    res.x = (-0.15 * p.x) + (0.28 * p.y);
    res.y = (0.26 * p.x) + (0.24 * p.y) + 0.44; // Apply transformation for f4
    return res;
}

int main() {
    // Initialize a starting point
    point a;

    // Set the starting point as part of the fern
    graph[0][0] = 1;

    // Iterate 500 million times to generate the fern
    for (unsigned long i = 0; i < 500000000; i++) {
        // Generate a random integer between 0 and 100
        int p = rand() % 100;

        // Choose which function to apply based on the random number
        if (p == 1) {
            a = f1(a); // Apply f1 if p is 1
        } else if (p < 86) {
            a = f2(a); // Apply f2 if p is between 2 and 85 inclusive
        } else if (p < 94) {
            a = f3(a); // Apply f3 if p is between 86 and 93 inclusive
        } else {
            a = f4(a); // Apply f4 if p is between 94 and 100 inclusive
        }

        // Plot the updated point on the image
        plot(a);
    }

    // Open a file to write the generated image data
    std::ofstream img("fern.ppm");

    // Write image header information
    img << "P3" << std::endl; // Specification identifier for PPM format
    img << width << " " << height << std::endl; // Image width and height
    img << "255" << std::endl; // Maximum value of color components

    // Loop through each pixel in the image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (graph[i][j] == 1) {
                // If the pixel is part of the fern, write dark green color
                img << "0 153 76" << std::endl;
            } else {
                // If the pixel is not part of the fern, write black color
                img << "0 0 0" << std::endl;
            }
        }
    }

    // Close the file
    img.close();

    return 0;
}
```

### References

- [Barnsley, Michael F, Fractals everywhere, Academic press, 2014.](https://books.google.com/books/about/Fractals_Everywhere.html?id=-wTjBQAAQBAJ)
- [Weisstein, Eric W. “Barnsley’s Fern.”](https://mathworld.wolfram.com/BarnsleysFern.html)
- [Barnsley Fern in Python - GeeksforGeeks](https://www.geeksforgeeks.org/barnsley-fern-in-python/)
