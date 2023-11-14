#include "raylib.h"
#include "../dynamic_arrays/dynamic_array.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

// INITIALIZE STATES
typedef enum {
    VIEW = 0,
    GRID_SELECT,
    POINT_SELECT,
	COUNT
} STATE;



// DECLARING PROGRAM DATA
typedef struct {
	Camera* main_camera;
    Camera* mini_camera;
    RenderTexture2D* reference_render;
    STATE* program_state;
    Associative_Array points;
	Associative_Array sel_points;
	Associative_Array face_points; // this part is controversial
    Vector3* save_vector;
    Vector3* grid_point;
    Vector3* grid_up;
	Color* colors;
} program_data;

void DRAW_COMPASS(RenderTexture2D*, Camera*);

void DRAW_POINTS(Vector3*, unsigned int, int*);

void DRAW_FACES(Vector3*, int*);

void VIEW_FUNCTION(program_data*);

void GRID_SELECT_FUNCTION(program_data*);

void POINT_SELECT_FUNCTION(program_data*);

int MOUSE_POINT_COLLISION(Ray, Vector3*, unsigned int);

void ADD_POINT(Vector3**, int**, unsigned int*, unsigned int*, Vector3);