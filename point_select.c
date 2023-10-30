#include "program_data.h"
#include "draw_util.h"
#include "raylib.h"
#include <stdlib.h>
#include "raymath.h"

void POINT_SELECT_FUNCTION(program_data* data) {

    // MOUSE COLLISION
    Ray mouse_ray = GetMouseRay(GetMousePosition(), *data->main_camera);


	    








	//Vector3 up = V3((data->save_vector->x == 1) ? 1 : 0, (data->save_vector->y == 1) ? 1 : 0, (data->save_vector->z == 1) ? 1 : 0);
	Vector3 upinverse = Vector3Subtract(V3(1, 1, 1), *data->grid_up);


	Vector3 corner1, corner2;
	if (upinverse.x == 1) {
		corner1 = Vector3Add(*data->grid_point, V3(0, 5, -5));
		corner2 = Vector3Add(*data->grid_point, V3(0, -5, 5));
	}
	if (upinverse.y == 1) {
		corner1 = Vector3Add(*data->grid_point, V3(5, 0, -5));
		corner2 = Vector3Add(*data->grid_point, V3(-5, 0, 5));
	}
	if (upinverse.z == 1) {
		corner1 = Vector3Add(*data->grid_point, V3(5, -5, 0));
		corner2 = Vector3Add(*data->grid_point, V3(-5, 5, 0));
	}
	
	RayCollision axis_collision = GetRayCollisionTriangle(mouse_ray,
			Vector3Add(*data->grid_point, V3((*data->grid_up).x * 5, (*data->grid_up).y * 5, (*data->grid_up).z * 5)),
			corner1,
			Vector3Subtract(*data->grid_point, V3((*data->grid_up).x * 5, (*data->grid_up).y * 5, (*data->grid_up).z * 5)));
	
	if (!axis_collision.hit) {
		axis_collision = GetRayCollisionTriangle(mouse_ray,
			Vector3Add(*data->grid_point, V3((*data->grid_up).x * 5, (*data->grid_up).y * 5, (*data->grid_up).z * 5)),
			corner2,
			Vector3Subtract(*data->grid_point, V3((*data->grid_up).x * 5, (*data->grid_up).y * 5, (*data->grid_up).z * 5)));
	}

	// snap point
	Vector3 draw_point = V3(closest_integer(axis_collision.point.x), closest_integer(axis_collision.point.y), closest_integer(axis_collision.point.z));

	if ((*data->grid_up).x == 0) draw_point.x = axis_collision.point.x;
	if ((*data->grid_up).y == 0) draw_point.y = axis_collision.point.y;
	if ((*data->grid_up).z == 0) draw_point.z = axis_collision.point.z;
	
    // HANDLE INPUT
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        DisableCursor();
    }
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        UpdateCamera(data->main_camera, CAMERA_THIRD_PERSON);
        data->mini_camera->position = Vector3Negate(Vector3Normalize(data->main_camera->position));
        data->mini_camera->up = data->main_camera->up;
    }
    if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
        EnableCursor();
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (axis_collision.hit) {
			
			//data->all_points[(*data->points_no)++] = draw_point;
			//*data->program_state = VIEW_MODE;
			
			//ADD_POINT(Vector3** points, unsigned int* points_no, unsigned int* points_length, Vector3 point)
			ADD_POINT(&data->all_points, &data->selected_points_idxs, data->points_no, data->points_length, draw_point);
			*data->program_state = VIEW_MODE;
		}
		
    }

	char buf[10];

	DRAW_COMPASS(data->reference_render, data->mini_camera);

    // DRAW MAIN SCREEN
    BeginDrawing();

    	//ClearBackground(STATE_COLORS[*(data->program_state)]);
		ClearBackground(data->colors[*(data->program_state)]);
		DrawRectangle(10, 10, GetScreenWidth() - 20, GetScreenWidth() - 20, GRAY);

		gcvt(draw_point.x, 1, buf);
		DrawText(buf, 0, 0, 30, BLACK);
		gcvt((*data->grid_up).x, 1, buf);
		DrawText(buf, 180, 0, 30, BLACK);

		gcvt(draw_point.y, 1, buf);
		DrawText(buf, 0, 30, 30, BLACK);
		gcvt((*data->grid_up).y, 1, buf);
		DrawText(buf, 180, 30, 30, BLACK);

		gcvt(draw_point.z, 1, buf);
		DrawText(buf, 0, 60, 30, BLACK);
		gcvt((*data->grid_up).z, 1, buf);
		DrawText(buf, 180, 60, 30, BLACK);


        BeginMode3D(*data->main_camera);
		

            // DRAW MAIN GRID
            draw_axis(V3(0, 0, 0), 10, BLACK, BLACK, BLACK);
			
			DRAW_POINTS(data->all_points, *data->points_no, data->selected_points_idxs);

			draw_grid2(*data->grid_point, 9, 1, *data->grid_up, BLACK);

			if (axis_collision.hit) DrawSphere(draw_point, 0.5, BLACK);

        EndMode3D();

        DrawTexture(data->reference_render->texture, 0, GetScreenWidth() - (GetScreenWidth() / 8), WHITE);

    EndDrawing();
    

}