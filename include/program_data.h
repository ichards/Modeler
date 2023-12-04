#include "raylib.h"
#include "../dynamic_arrays/dynamic_array.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

// INITIALIZE STATES
typedef enum {
    VIEW = 0,
    AXIS_SELECT,
    GRID_SELECT,
    POINT_SELECT,
    POINT_TRANSLATE,
	COUNT
} STATE;

typedef union interstate_data {
    Vector3 ivector;
    struct point_and_orientation {
        Vector3 point;
        Vector3 up;
    };
    size_t trans_point;
} idata;

// DECLARING PROGRAM DATA
typedef struct {
	Camera* main_camera;
    Camera* mini_camera;
    RenderTexture2D* reference_render;
    STATE* program_state;
    Associative_Array* points;
	Dynamic_Array* sel_points;
	Dynamic_Array* face_points; // this part is controversial
    Vector3* save_vector; // idata is gonna replace these 3 Vector3s
    Vector3* grid_point;
    Vector3* grid_up;
	Color* colors;
} program_data;

// Create face
typedef struct {
	size_t v1;
	size_t v2;
	size_t v3;
} face;

void DRAW_COMPASS(RenderTexture2D*, Camera*);

void DRAW_POINTS(Associative_Array, Dynamic_Array);

void DRAW_FACES(Associative_Array, Dynamic_Array);

void VIEW_FUNCTION(program_data*);

void AXIS_SELECT_FUNCTION(program_data*);

void GRID_SELECT_FUNCTION(program_data*);

void POINT_SELECT_FUNCTION(program_data*);

void POINT_TRANSLATE_FUNCTION(program_data*);

int MOUSE_POINT_COLLISION(Ray, Associative_Array);

size_t ADD_POINT(Associative_Array*, Vector3);

// DEBUG

int debug_per_s(int); // this is roughly used to print something every once in a while