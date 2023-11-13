#include "raylib.h"

// convenient functions for making vectors/colors
#define V3(x, y, z) ((Vector3) {(float) x, (float) y, (float) z})
#define C4(r, g, b, a) ((Color) {r, g, b, a})

Vector3 absolutify(Vector3);
void draw_grid(Vector3, int, float, Vector3, Color);
void draw_grid2(Vector3, int, float, Vector3, Color);
void draw_axis(Vector3, int, Color, Color, Color);
void draw_y_axis(Vector3, int, float);
void draw_quad(Vector3, Vector3, Vector3, Vector3, Color);
int closest_integer(float);