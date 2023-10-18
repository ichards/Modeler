#include "program_data.h"
#include "draw_util.h"
#include "raylib.h"

#include "raymath.h"

void POINT_SELECT_FUNCTION(program_data data) {

    // MOUSE COLLISION
    //Color x_color = BLACK, y_color = BLACK, z_color = BLACK;
    Ray mouse_ray = GetMouseRay(GetMousePosition(), *data.main_camera);
    //float distance;

    //RayCollision x_collision = GetRayCollisionBox(mouse_ray, (BoundingBox) {V3(-10, -0.5, -0.5), V3(10, 0.5, 0.5)});
    //RayCollision y_collision = GetRayCollisionBox(mouse_ray, (BoundingBox) {V3(-0.5, -10, -0.5), V3(0.5, 10, 0.5)});




	    
    

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

/*    RayCollision axis_collision = GetRayCollisionBox(mouse_ray, (BoundingBox)
		    {Vector3Add(*data.grid_point, V3(upinverse.x + (data.grid_up->x * 5), upinverse.y + (data.grid_up->x * 5), upinverse.z + (data.grid_up->x * 5))),
			    Vector3Subtract(*data.grid_point, V3(upinverse.x + (data.grid_up->x * 5), upinverse.y + (data.grid_up->x * 5), upinverse.z + (data.grid_up->x * 5))) 
			    });
			    */
	Vector3 corner1, corner2;
	if (upinverse.x == 0) {
		corner1 = Vector3Add(*data.grid_point, V3(0, 5, -5));
		corner2 = Vector3Add(*data.grid_point, V3(0, -5, 5));
	}
	if (upinverse.y == 0) {
		corner1 = Vector3Add(*data.grid_point, V3(5, 0, -5));
		corner2 = Vector3Add(*data.grid_point, V3(-5, 0, 5));
	}
	if (upinverse.z == 0) {
		corner1 = Vector3Add(*data.grid_point, V3(5, -5, 0));
		corner2 = Vector3Add(*data.grid_point, V3(-5, 5, 0));
	}

	RayCollision axis_collision = GetRayCollisionQuad(mouse_ray,
			Vector3Add(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)),
			Vector3Subtract(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)),
			corner1,
			corner2);


    // DRAW MAIN SCREEN
    BeginDrawing();

    	//ClearBackground(STATE_COLORS[*(data.program_state)]);
		ClearBackground(data.colors[*(data.program_state)]);
	DrawRectangle(10, 10, *(data.window_width) - 20, *(data.window_height) - 20, GRAY);

        BeginMode3D(*data.main_camera);

            // DRAW MAIN GRID
            draw_axis(V3(0, 0, 0), 10, BLACK, BLACK, BLACK);

 	    draw_grid2(*data.grid_point, 5, 1, *data.grid_up, BLACK);

	    DrawTriangle3D(Vector3Add(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)),
			Vector3Subtract(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)),
			corner1,
			BLUE);
	    DrawTriangle3D(corner1,
			Vector3Subtract(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)),
			Vector3Add(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)),
			BLUE);


	    DrawTriangle3D(Vector3Add(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)),
			Vector3Subtract(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)),
			corner2,
			BLUE);
	    DrawTriangle3D(corner2,
			Vector3Subtract(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)),
			Vector3Add(*data.grid_point, V3(up.x * 5, up.y * 5, up.z * 5)),
			BLUE);

	    DrawSphere(axis_collision.point, 1, BLACK);

        EndMode3D();

        DrawTexture(data.reference_render->texture, 0, *data.window_height - (*data.window_width / 8), WHITE);

    EndDrawing();
    

}