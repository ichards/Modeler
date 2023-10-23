#include "raylib.h"

// INITIALIZE STATES
typedef enum {
    VIEW_MODE = 0,
    POINT_MODE_SELECT,
    POINT_MODE_PLACE,
	COUNT
} STATE;



// DECLARING PROGRAM DATA
typedef struct {
    const int* window_width;
    const int* window_height;
    Camera* main_camera;
    Camera* mini_camera;
    RenderTexture2D* reference_render;
    STATE* program_state;
    Vector3* all_points;
	unsigned int* points_no;
    Vector3* save_vector;
    Vector3* grid_point;
    Vector3* grid_up;
	Color* colors;
} program_data;

void DRAW_COMPASS(RenderTexture2D*, Camera*);

void DRAW_POINTS(Vector3*, unsigned int);

void VIEW_FUNCTION(program_data data);

void GRID_SELECT_FUNCTION(program_data data);

void POINT_SELECT_FUNCTION(program_data data);