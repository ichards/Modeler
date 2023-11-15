#include "../include/program_data.h"
#include "../include/draw_util.h"
#include "../include/raylib.h"
#include "../include/raymath.h"

//void POINT_SELECT_FUNCTION(Camera* camera, Camera *mini_camera, RenderTexture2D* corner_render, const int* screenWidth, const int* screenHeight, STATE* mode) {
void GRID_SELECT_FUNCTION(program_data* data) {

    // MOUSE COLLISION
    //Color x_color = BLACK, y_color = BLACK, z_color = BLACK;
    Ray mouse_ray = GetMouseRay(GetMousePosition(), *data->main_camera);


    RayCollision axis_collision = GetRayCollisionBox(mouse_ray, (BoundingBox) {Vector3Multiply(V3(-0.5, -0.5, -0.5), *data->save_vector), Vector3Multiply(V3(0.5, 0.5, 0.5), *data->save_vector)});

    // make a visual sphere that shows where the user is selecting (assuming clip to nearest whole value)
    Vector3 input_point = V3(0, 0, 0);
    if (axis_collision.hit) {
        Vector3 signs = V3(((axis_collision.point.x > 0) - (axis_collision.point.x < 0)) * (data->save_vector->x == 20),
                ((axis_collision.point.y > 0) - (axis_collision.point.y < 0)) * (data->save_vector->y == 20),
                ((axis_collision.point.z > 0) - (axis_collision.point.z < 0)) * (data->save_vector->z == 20));
        input_point = V3((int)(axis_collision.point.x + (0.5 * signs.x)),(int)(axis_collision.point.y + (0.5 * signs.y)),(int)(axis_collision.point.z + (0.5 * signs.z)));
    }

	
 	Vector3 up = V3((data->save_vector->x == 1) ? 1 : 0, (data->save_vector->y == 1) ? 1 : 0, (data->save_vector->z == 1) ? 1 : 0);   

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
        //data->all_points[0] = input_point;
        *data->program_state = POINT_SELECT;
	*data->grid_point = input_point;
	*data->grid_up = up;
    }

	
	DRAW_COMPASS(data->reference_render, data->mini_camera);


    // DRAW MAIN SCREEN
    BeginDrawing();

    	//ClearBackground(STATE_COLORS[*(data->program_state)]);
		ClearBackground(data->colors[*(data->program_state)]);
	DrawRectangle(10, 10, GetScreenWidth() - 20, GetScreenWidth() - 20, GRAY);

        BeginMode3D(*data->main_camera);
		

            // DRAW MAIN GRID
            draw_axis(V3(0, 0, 0), 10, BLACK, BLACK, BLACK);
			
			DRAW_POINTS(*(data->points), *(data->sel_points));

			
            if (axis_collision.hit) {
                DrawSphere(input_point, 0.25, BLACK);
                //draw_grid(V3((int) (axis_collision.point.x), (int) (axis_collision.point.y), (int) (axis_collision.point.z)), 5, 1, up, BLACK);
				draw_grid(input_point, 5, 1, up, BLACK);

            }

        EndMode3D();

        DrawTexture(data->reference_render->texture, 0, GetScreenHeight() - (GetScreenWidth() / 8), WHITE);

    EndDrawing();
    

}