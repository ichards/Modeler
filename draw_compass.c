#include "program_data.h"
#include "draw_util.h"
#include <stdlib.h>
#include <string.h>

void DRAW_COMPASS(RenderTexture2D* render, Camera* camera) {
    // DRAW XYZ AXIS ONTO SEPERATE RENDER INSTANCE
    BeginTextureMode(*render);
        BeginMode3D(*camera);

            ClearBackground((Color){100, 100, 100, 255});

            DrawLine3D(V3(0, 0, 0), V3(-0.25, 0, 0), C4(255, 0, 0, 255));
            DrawLine3D(V3(0, 0, 0), V3(0.25, 0, 0), C4(0, 255, 255, 255));
            DrawCylinderEx(V3(-0.15, 0, 0), V3(-0.1, 0, 0), 0, 0.025, 10, C4(255, 0, 0, 255));
            DrawCylinderEx(V3(0.15, 0, 0), V3(0.1, 0, 0), 0.025, 0, 10, C4(0, 255, 255, 255));

            DrawLine3D(V3(0, 0, 0), V3(0, -0.25, 0), C4(0, 255, 0, 255));
            DrawLine3D(V3(0, 0, 0), V3(0, 0.25, 0), C4(255, 0, 255, 255));
            DrawCylinderEx(V3(0, -0.15, 0), V3(0, -0.1, 0), 0, 0.025, 10, C4(0, 255, 0, 255));
            DrawCylinderEx(V3(0, 0.15, 0), V3(0, 0.1, 0), 0.025, 0, 10, C4(255, 0, 255, 255));

            DrawLine3D(V3(0, 0, 0), V3(0, 0, -0.25), C4(0, 0, 255, 255));
            DrawLine3D(V3(0, 0, 0), V3(0, 0, 0.25), C4(255, 255, 0, 255));
            DrawCylinderEx(V3(0, 0, -0.15), V3(0, 0, -0.1), 0, 0.025, 10, C4(0, 0, 255, 255));
            DrawCylinderEx(V3(0, 0, 0.15), V3(0, 0, 0.1), 0.025, 0, 10, C4(255, 255, 0, 255));

        EndMode3D();
    EndTextureMode();
}

void DRAW_POINTS(Vector3* points, unsigned int points_no, int* selected_points_idxs) {
	for (unsigned int i=0; i<points_no; i++) {
		DrawSphere(points[i], 0.5, BLACK);
	}
	unsigned int i = -1;
	while (selected_points_idxs[++i] != -1) {
		DrawSphere(points[selected_points_idxs[i]], 0.6, BLUE);
	}
}

void ADD_POINT(Vector3** points, unsigned int* points_no, unsigned int* points_length, Vector3 point) {
	
	// need to resize
	if (*points_no >= *points_length) {
		Vector3* new_points = malloc(sizeof(Vector3) * (*points_length * 2));
		memset(new_points, 0, *points_length * 2);
		for (unsigned int i=0; i<*points_length; i++) {
			new_points[i] = (*points)[i];
		}
		free(*points);
		*points = new_points;
		*points_length *= 2;
	}
	
	(*points)[*points_no++] = point;
	
	
}