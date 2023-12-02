#include "../include/program_data.h"
#include "../include/draw_util.h"
#include "../include/raylib.h"
#include "../dynamic_arrays/dynamic_array.h"
#include "../include/raymath.h"

#include <stdio.h>

//void VIEW_FUNCTION(Camera* camera, Camera *mini_camera, RenderTexture2D* corner_render, const int* screenWidth, const int* screenHeight, STATE* mode) {
void VIEW_FUNCTION(program_data* data) {

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

    if (IsKeyPressed(KEY_A)) {
        *data->program_state = AXIS_SELECT;
    }
	
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		
		int click_point = MOUSE_POINT_COLLISION(GetMouseRay(GetMousePosition(), *data->main_camera), *(data->points));

        if (click_point == -1) {
            //for (int i=0; i<data->sel_points->current_length; i++) {
            if (data->sel_points->current_length > 0) {
                da_clear(data->sel_points);
            }
        } else {
            printf("clicked point\n");
            size_t click_point_s = click_point;
            da_push(data->sel_points, (void*)&click_point_s);
            printf("sel_points now %d large\n", data->sel_points->current_length);
        }
    }

    if (IsKeyPressed(KEY_F)) {
        // validate to make sure 3 points are selected
        if (data->sel_points->current_length == 3) {
            size_t* pointp = (size_t*) data->sel_points->p;
            face new_face = {pointp[0], pointp[1], pointp[2]};
            DA_PUSH(data->face_points, face, new_face);
            DAP(facep, face, data->face_points->p);
        }
    }

    if (IsKeyPressed(KEY_D)) {
        // delete a point
        // this will decimate the program if the points are used for a face, gotta be careful
        for (size_t i=0; i<data->sel_points->current_length; i++) {
            size_t* spointsp = (size_t*) data->sel_points->p;
            ada_remove(data->points, spointsp[i]);
        }
        da_clear(data->sel_points);
    }

    if (IsKeyPressed(KEY_T) && data->sel_points->current_length == 1) {
        *(data->program_state) = POINT_TRANSLATE;
    }

    // END INPUT

    // DRAW MAIN SCREEN
	DRAW_COMPASS(data->reference_render, data->mini_camera);

    BeginDrawing();

    	//ClearBackground(STATE_COLORS[*(data->program_state)]);
		ClearBackground(data->colors[*(data->program_state)]);

		DrawRectangle(10, 10, GetScreenWidth() - 20, GetScreenWidth() - 20, GRAY);

        BeginMode3D(*data->main_camera);

            // DRAW MAIN GRID
            draw_axis(V3(0, 0, 0), 10, BLACK, BLACK, BLACK);
			
			DRAW_POINTS(*(data->points), *(data->sel_points));

            DRAW_FACES(*(data->points), *(data->face_points));

        EndMode3D();

        DrawTexture(data->reference_render->texture, 0, GetScreenHeight() - (GetScreenWidth() / 8), WHITE);


    EndDrawing();
    

}