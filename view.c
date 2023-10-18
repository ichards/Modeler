#include "program_data.h"
#include "draw_util.h"
#include "raylib.h"

#include "raymath.h"

//void VIEW_FUNCTION(Camera* camera, Camera *mini_camera, RenderTexture2D* corner_render, const int* screenWidth, const int* screenHeight, STATE* mode) {
void VIEW_FUNCTION(program_data data) {

    // MOUSE COLLISION
    Color x_color = BLACK, y_color = BLACK, z_color = BLACK;
    Ray mouse_ray = GetMouseRay(GetMousePosition(), *data.main_camera);
    float distance;

    RayCollision x_collision = GetRayCollisionBox(mouse_ray, (BoundingBox) {V3(-10, -0.5, -0.5), V3(10, 0.5, 0.5)});
    RayCollision y_collision = GetRayCollisionBox(mouse_ray, (BoundingBox) {V3(-0.5, -10, -0.5), V3(0.5, 10, 0.5)});
    RayCollision z_collision = GetRayCollisionBox(mouse_ray, (BoundingBox) {V3(-0.5, -0.5, -10), V3(0.5, 0.5, 10)});

    *data.save_vector = V3(1, 1, 1); // THE VARIABLE THAT WILL BE SENT TO THE NEXT STATE

    if (x_collision.hit) {
        x_color = WHITE;
        data.save_vector->x = 20;
    }

    if (y_collision.hit) {
        x_color = BLACK;
        y_color = WHITE;
        data.save_vector->x = 1;
        data.save_vector->y = 20;
        if (x_collision.hit && x_collision.distance < y_collision.distance) {
            x_color = WHITE;
            y_color = BLACK;
            data.save_vector->x = 20;
            data.save_vector->y = 1;
        }
    }

    if (z_collision.hit) {
        x_color = BLACK;
        y_color = BLACK;
        z_color = WHITE;
        data.save_vector->z = 20;

        if (x_collision.hit && x_collision.distance < z_collision.distance) {
            x_color = WHITE;
            z_color = BLACK;
            data.save_vector->x = 20;
            data.save_vector->z = 1;
        }
        if (y_collision.hit && y_collision.distance < z_collision.distance) {
            y_color = WHITE;
            z_color = BLACK;
            data.save_vector->y = 20;
            data.save_vector->z = 1;
        }
    }


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

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (x_collision.hit || y_collision.hit || z_collision.hit) {
            if (*data.program_state == VIEW_MODE) {
                *data.program_state = POINT_MODE_SELECT;
            }
        }
    }

	DRAW_COMPASS(data.reference_render, data.mini_camera);





    // DRAW MAIN SCREEN
    BeginDrawing();

    	//ClearBackground(STATE_COLORS[*(data.program_state)]);
		ClearBackground(data.colors[*(data.program_state)]);

	DrawRectangle(10, 10, *(data.window_width) - 20, *(data.window_height) - 20, GRAY);

        BeginMode3D(*data.main_camera);

            // DRAW MAIN GRID
            draw_axis(V3(0, 0, 0), 10, x_color, y_color, z_color);

            DrawSphere(data.all_points[0], 0.5, BLACK);

        EndMode3D();

        DrawTexture(data.reference_render->texture, 0, *data.window_height - (*data.window_width / 8), WHITE);

    EndDrawing();
    

}