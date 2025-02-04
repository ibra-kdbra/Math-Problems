# Purpose

This project is a simple math plotter using ImGui for the user interface and ImPlot for visualizing mathematical functions. It allows the user to input mathematical formulas, adjust axis ranges, and visualize the function's graph in real-time.

## What Does It Do

- Initializes a GLFW window with OpenGL context.
- Loads ImGui and ImPlot for creating a graphical interface.
- Accepts user input for a mathematical expression and plots the function.
- Provides interactive controls for adjusting the plot's axis ranges.
- Dynamically updates the plot based on the formula entered by the user.
  
The program also features the use of the exprtk library for parsing and evaluating mathematical expressions. It allows users to input custom formulas for plotting.

## Usage

```shell
mkdir build && cd build
conan install ..
cmake ..
cmake --build .
```

### Screenshot

![equa_plot](imgs/plot.gif)

### Libraries Versions

- imgui/1.83
- implot/0.11
- glfw/3.3.8
- glew/2.2.0
- exprtk
