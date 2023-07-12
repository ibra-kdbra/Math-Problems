# mandelbrot

SFML-based Mandelbrot viewer program.

<p float="middle">
    <img src="imgs/mandelbrot.png" width="300"/>
    <img src="imgs/zoomed.png"     width="300"/>
</p>

# Requirements
 * C++17
 * CMake 3.22

# Building & Running

```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --target run
```

# Controls

| Action            | Control         |
| ----------------- | --------------- |
| Go to point       | Click           |
| Zoom              | Scroll (or W/S) |
| Pan               | Arrow keys      |
| Change iterations | [ and ]         |
| Reset view        | R               |
