#include "../include/program_data.h"
#include "../include/draw_util.h"
#include "../include/raymath.h"

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
    
    Ray mouse_ray = GetMouseRay(GetMousePosition(), *data->main_camera);
    DA_DUPLICATE_VAL(data->points->vals, data->general_data->trans_point, Vector3, state_point)

    Vector3 x1 = Vector3Add(state_point, V3(-10, -0.5, -0.5));
    Vector3 x2 = Vector3Add(state_point, V3(10, 0.5, 0.5));
    
    Vector3 y1 = Vector3Add(state_point, V3(-0.5, -10, -0.5));
    Vector3 y2 = Vector3Add(state_point, V3(0.5, 10, 0.5));
    
    Vector3 z1 = Vector3Add(state_point, V3(-0.5, -0.5, -10));
    Vector3 z2 = Vector3Add(state_point, V3(0.5, 0.5, 10));

    // let's make the collision boxes for the axis
    RayCollision collision[] = {
        GetRayCollisionBox(mouse_ray, (BoundingBox) {x1, x2}),
        GetRayCollisionBox(mouse_ray, (BoundingBox) {y1, y2}),
        GetRayCollisionBox(mouse_ray, (BoundingBox) {z1, z2})
    };

    Color axis_colors[] = {
        BLACK, BLACK, BLACK
    };

    // these are for getting slope when translating
    Vector3 p1s[] = {
        V3(1, 0, 0),
        V3(0, 1, 0),
        V3(0, 0, 1)
    };

    Vector3 p2s[] = {
        V3(-1, 0, 0),
        V3(0, -1, 0),
        V3(0, 0, -1)
    };

    // selected axis
    float dist = 0;
    int select = -1;
    for (int i=0; i<3; i++) {
        if (collision[i].hit) {
            if (select == -1) {
                dist = collision[i].distance;
                select = i;
            } else {
                if (collision[i].distance < dist) {
                    dist = collision[i].distance;
                    select = i;
                }
            }
        }
    }

    if (select != -1) {
        axis_colors[select] = WHITE;
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (select != -1) {
            // axis is selected and user is clicking
            Vector2 line2d = Vector2Subtract(GetWorldToScreen(p1s[select]), GetWorldToScreen(p2s[select]));
            float slope = line2d.y / line2d.x;

            float mouseslope = GetMouseDelta().y / GetMouseDelta().x;
            float mouselength = Vector2Length(GetMouseDelta());

            // TODO: convert slope to degrees/radians, and then get scalar value between 0 and 1 comparing line slope and mouse slope

            ((Vector3*)(data->points->vals.p))[data->general_data->trans_point].x += mouselength * ()

            // temporary... ;)
            //((Vector3*)(data->points->vals.p))[data->general_data->trans_point].x += GetMouseDelta().x;
            //GetMouseDelta();
        }
    } else {
        // rotating camera while translating point will cause chaos
        // keep in mind the user left clicking does not necessarily mean they're translating it, but whatever
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
    }

    


	DRAW_COMPASS(data->reference_render, data->mini_camera);

    BeginDrawing();

		ClearBackground(data->colors[*(data->program_state)]);

		DrawRectangle(10, 10, GetScreenWidth() - 20, GetScreenWidth() - 20, GRAY);

        BeginMode3D(*data->main_camera);

            // DRAW MAIN GRID
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
            draw_axis(state_point, 10, axis_colors[0], axis_colors[1], axis_colors[2]);


            DRAW_FACES(*(data->points), *(data->face_points));

        EndMode3D();

        DrawTexture(data->reference_render->texture, 0, GetScreenHeight() - (GetScreenWidth() / 8), WHITE);


    EndDrawing();
}