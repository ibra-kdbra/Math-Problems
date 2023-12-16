#include <fstream>
#include <math.h>
#include <vector>
#include <iostream> //remove after debugging
#include <complex>

#define PREC 5
#define MAX_ITER 100000
#define WIDTH 800
#define HEIGHT 800

typedef unsigned int u32;
typedef unsigned char u8;
typedef double db;

db setP(db num) {
  num = (int)(num * (pow(10,PREC)));
  db res = (db)(num / pow(10,PREC));
  return res;
}

db iterate(const std::complex<db>& zorc) {
  std::complex<db> z(0.0, 0.0);
  db iter = 0;
  while (abs(z) <= 2.0 && iter < MAX_ITER) {
    db x = abs(z.real());
    db y = abs(z.imag());
    z = std::complex<db>(x, y) * std::complex<db>(x, y) + zorc;
    ++iter;
  }
  if (iter < MAX_ITER) {
    db log_zn = log(z.real()*z.real() + z.imag()*z.imag()) / 2;
    db nu = log(log_zn/log(2)) / log(2);
    iter = iter + 1 - nu;
  }
  return iter;
}

// Define a color palette
std::vector<std::vector<u8>> palette = {
  {66, 30, 15},
  {25, 7, 26},
  {9, 1, 47},
  {4, 4, 73},
  {0, 7, 100},
  {12, 44, 138},
  {24, 82, 177},
  {57, 125, 209},
  {134, 181, 229},
  {211, 236, 248},
  {241, 233, 191},
  {248, 201, 95},
  {255, 170, 0},
  {204, 128, 0},
  {153, 87, 0},
  {106, 52, 3}
};

int main() {
  std::ofstream ofs("output.ppm");
  ofs << "P3\n" << WIDTH << ' ' << HEIGHT << ' ' << "255\n";
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      db real = (db)x / WIDTH * 4.0 - 2.0;
      db imag = (db)y / HEIGHT * 4.0 - 2.0;
      std::complex<db> c(real, imag);
      db value = iterate(c);
      // Use the escape time to index into the color palette
      std::vector<u8> color1 = palette[(int)value % palette.size()];
      std::vector<u8> color2 = palette[((int)value+1) % palette.size()];
      db t = value - (int)value; // Get fractional part of value
      // Interpolate between the two colors
      std::vector<u8> color = {
        (u8)(color1[0] * (1-t) + color2[0] * t),
        (u8)(color1[1] * (1-t) + color2[1] * t),
        (u8)(color1[2] * (1-t) + color2[2] * t)
      };
      ofs << (int)color[0] << ' ' << (int)color[1] << ' ' << (int)color[2] << '\n';
    }
  }
  ofs.close();
  std::cout << "Fractal generated and saved to 'output.ppm'.\n";
}
