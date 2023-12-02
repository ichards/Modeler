#include "../include/program_data.h"
#include "../include/draw_util.h"

// alright here's the idea
// you click on a point and enter translate mode. the three axes appear on the point.
// when you click on one, they disappear and a very long one appears on the one you clicked.
// keep in mind you're holding your left mouse button down this whole time.
// anyways, if you move your (now invisible mouse) in the direction of the axis, the point moves.
// but wait, there's MORE.
// if you move your mouse away from the axis you selected, it decreases the size of the unit of which the grid snaps.
// this means you could move a point 0.5 units, or 0.25 units in a certain direction.
// that's all. unimpressed? you can leave.

void POINT_TRANSLATE_FUNCTION(program_data* data) {


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