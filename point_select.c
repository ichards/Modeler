#include "program_data.h"
#include "draw_util.h"
#include "raylib.h"
#include <stdlib.h>
#include "raymath.h"

void POINT_SELECT_FUNCTION(program_data data) {

    // MOUSE COLLISION
    Ray mouse_ray = GetMouseRay(GetMousePosition(), *data.main_camera);


	    


    // HANDLE INPUT
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        DisableCursor();
    }
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        UpdateCamera(data.main_camera, CAMERA_THIRD_PERSON);
        data.mini_camera->position = Vector3Negate(Vector3Normalize(data.main_camera->position));
        data.mini_camera->up = data.main_camera->up;
    }
    if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
        EnableCursor();
    }

	DRAW_COMPASS(data.reference_render, data.mini_camera);




	Vector3 up = V3((data.save_vector->x == 1) ? 1 : 0, (data.save_vector->y == 1) ? 1 : 0, (data.save_vector->z == 1) ? 1 : 0);
	Vector3 upinverse = Vector3Subtract(V3(1, 1, 1), *data.grid_up);


	Vector3 corner1, corner2;
	if (upinverse.x == 1) {
		corner1 = Vector3Add(*data.grid_point, V3(0, 5, -5));
		corner2 = Vector3Add(*data.grid_point, V3(0, -5, 5));
	}
	if (upinverse.y == 1) {
		corner1 = Vector3Add(*data.grid_point, V3(5, 0, -5));
		corner2 = Vector3Add(*data.grid_point, V3(-5, 0, 5));
	}
	if (upinverse.z == 1) {
		corner1 = Vector3Add(*data.grid_point, V3(5, -5, 0));
		corner2 = Vector3Add(*data.grid_point, V3(-5, 5, 0));
	}
	
	RayCollision axis_collision = GetRayCollisionTriangle(mouse_ray,
			Vector3Add(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)),
			corner1,
			Vector3Subtract(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)));
	
	if (!axis_collision.hit) {
		axis_collision = GetRayCollisionTriangle(mouse_ray,
			Vector3Add(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)),
			corner2,
			Vector3Subtract(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)));
	}

	// snap point
	Vector3 axis_collision_nosnap = V3(0, 0, 0);
	if (axis_collision.hit) {
		Vector3 signs = V3(
			((*data.grid_point).x > 0) - ((*data.grid_point).x < 0),
			((*data.grid_point).y > 0) - ((*data.grid_point).y < 0),
			((*data.grid_point).z > 0) - ((*data.grid_point).z < 0)
		);
		axis_collision_nosnap = V3((axis_collision.point.x + (0.5 * signs.x)),(axis_collision.point.y + (1.5 * signs.y)),(axis_collision.point.z + (0.5 * signs.z)));
		axis_collision.point = V3((int)(axis_collision.point.x + (0.5 * signs.x)),(int)(axis_collision.point.y + (1.5 * signs.y)),(int)(axis_collision.point.z + (0.5 * signs.z)));
	}


	char buf[10];

    // DRAW MAIN SCREEN
    BeginDrawing();

    	//ClearBackground(STATE_COLORS[*(data.program_state)]);
		ClearBackground(data.colors[*(data.program_state)]);
		DrawRectangle(10, 10, *(data.window_width) - 20, *(data.window_height) - 20, GRAY);

		gcvt(axis_collision_nosnap.x, 1, buf);
		DrawText(buf, 0, 0, 30, BLACK);
		gcvt(axis_collision.point.x, 1, buf);
		DrawText(buf, 180, 0, 30, BLACK);

		gcvt(axis_collision_nosnap.y, 1, buf);
		DrawText(buf, 0, 30, 30, BLACK);
		gcvt(axis_collision.point.y, 1, buf);
		DrawText(buf, 180, 30, 30, BLACK);

		gcvt(axis_collision_nosnap.z, 1, buf);
		DrawText(buf, 0, 60, 30, BLACK);
		gcvt(axis_collision.point.z, 1, buf);
		DrawText(buf, 180, 60, 30, BLACK);


        BeginMode3D(*data.main_camera);

            // DRAW MAIN GRID
            draw_axis(V3(0, 0, 0), 10, BLACK, BLACK, BLACK);

 	    draw_grid2(*data.grid_point, 5, 1, *data.grid_up, BLACK);

	    DrawSphere(axis_collision.point, 0.5, BLACK);

        EndMode3D();

        DrawTexture(data.reference_render->texture, 0, *data.window_height - (*data.window_width / 8), WHITE);

    EndDrawing();
    

}