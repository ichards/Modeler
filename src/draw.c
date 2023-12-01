#include "../include/program_data.h"
#include "../include/draw_util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void draw_single_points(void* val, size_t idx) {
	DAP(valp, Vector3, val);
	DrawSphere(valp[idx], 0.3, BLACK);
	if (debug_per_s(0)) {
		printf("[%d] - %.1f, %.1f, %.1f\n", idx, valp[idx].x, valp[idx].y, valp[idx].z);
	}
}

void DRAW_POINTS(Associative_Array points, Dynamic_Array sel_points) {
	ada_iter(points, draw_single_points);
	for (size_t i=0; i<sel_points.current_length; i++) {
		DAP(cur_idx, size_t, sel_points.p);
		DrawSphere(INDAP(Vector3, points.vals.p)[cur_idx[i]], 0.4, RED);
	}
}

void DRAW_FACES(Associative_Array points, Dynamic_Array faces) {

	face* faces_p = (face*) faces.p;

	face current_face = {0};
	Vector3* points_p = (Vector3*) points.vals.p;

	for (size_t i=0; i<faces.current_length; i++) {
		current_face = faces_p[i];
		DrawTriangle3D(points_p[current_face.v1], points_p[current_face.v2], points_p[current_face.v3], BLUE);
		DrawTriangle3D(points_p[current_face.v3], points_p[current_face.v2], points_p[current_face.v1], BLUE);
	}
}

size_t ADD_POINT(Associative_Array* points, Vector3 point) {

	return ada_push(points, &point);
	
}

// debug nonsense
static int timer = 0;
int debug_per_s(int addend) {
	timer += addend;
	int ret = 0;
	if (timer >= 120) {
		ret = 1;
	}
	if (timer >= 121) {
		timer = 0;
	}
	return ret;
}