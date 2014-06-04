#ifndef PLANE_H
#define PLANE_H

#define planeVertex0 vec4(-1.0f/2, 0, -1.0f/2, 1)
#define planeVertex1 vec4(-1.0f/2, 0,  1.0f/2, 1)
#define planeVertex2 vec4( 1.0f/2, 0,  1.0f/2, 1)
#define planeVertex3 vec4( 1.0f/2, 0, -1.0f/2, 1)

#include "Rigidbody.h"

class Plane : public Rigidbody {
public:
	float p0[3];
	float p1[3];
	float p2[3];
	float p3[3];
	Plane(vec3 planePosition, vec3 planeRotation, vec3 planeVelocity, float planeSize, float planeMass, vec3 planeColor);
	~Plane();
	void inline render();
};
#endif // !PLANE_H