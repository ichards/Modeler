#include "program_data.h"

// point_cnt > points_no
// returns pointer to sphere ray collides with, null if nothing. point is closest point
Vector3* MOUSE_POINT_COLLISION(Ray mouse_ray, Vector3* points, unsigned int points_no) {
	Vector3* hit_point = NULL;
	float min_distance = 0;
	for (int i=0; i<points_no; i++) {
		RayCollision mouse_point = GetRayCollisionSphere(mouse_ray, points[i], 0.5);
		if (mouse_point.hit) {
			if (hit_point == NULL) {
				hit_point = points+i;
				min_distance = mouse_point.distance;
			} else if (mouse_point.distance < min_distance) {
				hit_point = points+i;
				min_distance = mouse_point.distance;
			}
		}
	}
	
	return hit_point;
}