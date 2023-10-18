#include "draw_util.h"
#include "raymath.h"

Vector3 absolutify(Vector3 v) {
	Vector3 buffer;
	buffer.x = v.x;
	buffer.y = v.y;
	buffer.z = v.z;
	if (v.x < 0) {
		buffer.x *= -1;
	}
	if (v.y < 0) {
		buffer.y *= -1;
	}
	if (v.z < 0) {
		buffer.z *= -1;
	}
	return buffer;
}

// draws grid
// instead of drawing grid now, it will draw the 2 complementary axes
void draw_grid(Vector3 center, int lines_no, float unit_size, Vector3 up, Color grid_color) {	
	if (up.x == 1) DrawLine3D(
			V3(center.x - (unit_size * lines_no), center.y, center.z),
			V3(center.x + (unit_size * lines_no), center.y, center.z),
			grid_color);
	if (up.y == 1) DrawLine3D(
			V3(center.x, center.y + (unit_size * lines_no), center.z),
			V3(center.x, center.y - (unit_size * lines_no), center.z),
			grid_color);
	if (up.z == 1) DrawLine3D(
			V3(center.x, center.y, center.z + (unit_size * lines_no)),
			V3(center.x, center.y, center.z - (unit_size * lines_no)),
			grid_color);
}

// actually draws grid. i'm such an idiot, i'll fix the naming later
void draw_grid2(Vector3 center, int lines_no, float unit_size, Vector3 up, Color grid_color) {	

	if (up.x == 1) {
		for (int i = -(lines_no/2); i<=lines_no/2; i++) {
			DrawLine3D(
			V3(center.x - (unit_size * lines_no / 2), center.y + (i * unit_size * up.y), center.z + (i * unit_size * up.z)),
			V3(center.x + (unit_size * lines_no / 2), center.y + (i * unit_size * up.y), center.z + (i * unit_size * up.z)),
			grid_color);
		}
	}
	if (up.y == 1) {
		for (int i = -(lines_no/2); i<=lines_no/2; i++) {
			DrawLine3D(
			V3(center.x + (i * unit_size * up.x), center.y + (unit_size * lines_no / 2), center.z + (i * unit_size * up.z)),
			V3(center.x + (i * unit_size * up.x), center.y - (unit_size * lines_no / 2), center.z + (i * unit_size * up.z)),
			grid_color);
		}
	}
	if (up.z == 1) {
		for (int i= -(lines_no/2); i<=lines_no/2; i++) {
			DrawLine3D(
			V3(center.x + (i * unit_size * up.x), center.y + (i * unit_size * up.y), center.z + (unit_size * lines_no / 2)),
			V3(center.x + (i * unit_size * up.x), center.y + (i * unit_size * up.y), center.z - (unit_size * lines_no / 2)),
			grid_color);
		}
	}
}

void draw_axis(Vector3 center, int unit_no, Color x, Color y, Color z) {
    DrawLine3D(Vector3Subtract(center, V3(unit_no, 0, 0)), Vector3Add(center, V3(unit_no, 0, 0)), x);
    DrawLine3D(Vector3Subtract(center, V3(0, unit_no, 0)), Vector3Add(center, V3(0, unit_no, 0)), y);
    DrawLine3D(Vector3Subtract(center, V3(0, 0, unit_no)), Vector3Add(center, V3(0, 0, unit_no)), z);
}

// draws y axis
void draw_y_axis(Vector3 center, int lines_no, float unit_size) {
    DrawLine3D(V3(center.x, center.y - ((lines_no/2) * unit_size), center.z), V3(center.x, center.y + ((lines_no/2) * unit_size), center.z), BLACK);
    for (int i= -(lines_no/2); i<=lines_no/2; i++) {
        DrawLine3D(V3(center.x - (unit_size / 2), center.y + (i * unit_size), center.z), V3(center.x + (unit_size / 2), center.y + (i * unit_size), center.z), BLACK);
        DrawLine3D(V3(center.x, center.y + (i * unit_size), center.z - (unit_size / 2)), V3(center.x, center.y + (i * unit_size), center.z + (unit_size / 2)), BLACK);
    }
}
