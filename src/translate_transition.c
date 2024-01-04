#include "../include/program_data.h"
#include "../include/draw_util.h"
#include "../include/raylib.h"
#include "../include/raymath.h"

void TRANSLATE_TRANSITION_FUNCTION(program_data* data) {

    DA_DUPLICATE_VAL(data->points->vals, data->general_data->trans_point, Vector3, state_point)

    Vector3 x1 = Vector3Add(state_point, V3(-10, -0.5, -0.5));
    Vector3 x2 = Vector3Add(state_point, V3(10, 0.5, 0.5));
    
    Vector3 y1 = Vector3Add(state_point, V3(-0.5, -10, -0.5));
    Vector3 y2 = Vector3Add(state_point, V3(0.5, 10, 0.5));
    
    Vector3 z1 = Vector3Add(state_point, V3(-0.5, -0.5, -10));
    Vector3 z2 = Vector3Add(state_point, V3(0.5, 0.5, 10));

    const float trans_time = 0.3;

    // now that i think about it, idk how this works
    static float timer = trans_time;

    // temporary, this should be in program_data
    static Vector3 init_camera_pos = {0};
    if (timer == trans_time) {
        init_camera_pos = data->main_camera->position;
    }

    data->main_camera->target = Vector3Scale(state_point, (trans_time - timer) / 1.0 / trans_time);

    data->main_camera->position = Vector3Add(init_camera_pos, Vector3Scale(state_point, (trans_time - timer) / 1.0 / trans_time));
    // TO-DO - change position aswell
    //data->main_camera->position = Vector3Scale()

    timer -= GetFrameTime();

    if (timer <= 0) {
        timer = 1.0;
        *(data->program_state) = POINT_TRANSLATE;
    }

    BeginDrawing();

		ClearBackground(data->colors[*(data->program_state)]);

		DrawRectangle(10, 10, GetScreenWidth() - 20, GetScreenWidth() - 20, GRAY);

        BeginMode3D(*data->main_camera);

            // DRAW MAIN GRID
            //draw_axis(V3(0, 0, 0), 10, BLACK, BLACK, BLACK);

            DrawSphere(x1, 0.3, RED);
            DrawSphere(x2, 0.3, RED);
            DrawSphere(y1, 0.3, GREEN);
            DrawSphere(y2, 0.3, GREEN);
            DrawSphere(z1, 0.3, BLUE);
            DrawSphere(z2, 0.3, BLUE);

			
			DRAW_POINTS(*(data->points), *(data->sel_points));

            DAP(cur_idx, size_t, data->sel_points->p);
            DrawSphere(INDAP(Vector3, data->points->vals.p)[cur_idx[0]], 0.6, YELLOW);
            //draw_axis(INDAP(Vector3, data->points->vals.p)[cur_idx[0]], 10, RED, GREEN, BLUE);
            draw_axis(state_point, 10, RED, GREEN, BLUE);


            DRAW_FACES(*(data->points), *(data->face_points));

        EndMode3D();

        DrawTexture(data->reference_render->texture, 0, GetScreenHeight() - (GetScreenWidth() / 8), WHITE);


    EndDrawing();
}