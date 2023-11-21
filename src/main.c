
#include <string.h>
#include <stdlib.h>
#include "../include/draw_util.h"
#include "../include/program_data.h"

#include "../include/raylib.h"
#include "../include/raymath.h"

#include "../dynamic_arrays/dynamic_array.h"

// RUN FUNCTIONS
// add a union parameter that lets you add input to the next state so that
// it stores minimal data that's interpreted depending on the state
//typedef void (*STATE_FUNCTION) (Camera*, Camera*, RenderTexture2D*, const int*, const int*, STATE*);
typedef void (*STATE_FUNCTION) (program_data*);



int main(void)
{

    // INITIALIZE WINDOW
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - core world screen");

    // INITIALIZE CAMERA
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Camera mini_camera = camera;
    mini_camera.position = Vector3Negate(Vector3Normalize(camera.position));

    // INITIALIZE PROGRAM STATE
    STATE mode = VIEW;
    STATE_FUNCTION mode_functions[] = {&VIEW_FUNCTION, &GRID_SELECT_FUNCTION, &POINT_SELECT_FUNCTION};

    // INITIALIZE RENDERING NONSENSE
    SetTargetFPS(60);

    RenderTexture2D corner_render = LoadRenderTexture(screenWidth / 8, screenWidth / 8);

	/* old stuff.... no good
	unsigned int points_length = 10;

    Vector3* all_points = malloc(points_length * sizeof(Vector3));
    memset(all_points, 0, points_length * sizeof(Vector3));
	
	unsigned int points_no = 0;
	
	int* selected_points_idxs = malloc(points_length * sizeof(int));
	memset(selected_points_idxs, -1, points_length * sizeof(int));
	
	int* face_idxs = malloc(points_length * 2 * sizeof(int));
	memset(face_idxs, -1, points_length * 2 * sizeof(int));
*/
	
	// the new stuff
	Vector3 point_default = V3(0, 0, 0);
	Associative_Array all_points = create_ada(create_da(malloc(sizeof(Vector3) * 8), &point_default, sizeof(Vector3), 0, 8));

	size_t sel_point_default = 0;
	Dynamic_Array sel_points = create_da(malloc(sizeof(size_t) * 8), &sel_point_default, sizeof(size_t), 0, 8);
	
	size_t face_point_default = 0;
	Dynamic_Array face_points = create_da(malloc(sizeof(size_t) * 8), &face_point_default, sizeof(size_t), 0, 8);
	
    Vector3 save_vector = V3(0, 0, 0);

    Vector3 grid_point = V3(0, 0, 0);
    Vector3 grid_up = V3(0, 0, 0);
	
	Color STATE_COLORS[] = {
		BLACK,	// VIEW
		RED,	// GRID_SELECT
		PURPLE	// POINT_SELECT
	};

    program_data p_program_data = (program_data) {&camera, &mini_camera, &corner_render, &mode, &all_points, &sel_points, &face_points, &save_vector, &grid_point, &grid_up, STATE_COLORS};

    // MAIN LOOP
    while (!WindowShouldClose())
    {

        mode_functions[mode](&p_program_data);

    }

    CloseWindow();
 
    return 0;
}

