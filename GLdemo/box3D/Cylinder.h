#ifndef CYLINDER_H
#define CYLINDER_H

#include "Rigidbody.h"

class Cylinder : public Rigidbody {
public:
	float radius;
	float length;
	Cylinder(vec3 cylinderPosition, vec3 cylinderRotation, vec3 cylinderVelocity, float cylinderRadius, float cylinderLength, float cylinderMass, vec3 cylinderColor);
	~Cylinder();
	vec3 inline getSkin();
	vec4 getBasePoint();
	vec4 getTopPoint();
	mat4 inline getRotationMatrixRender();
	mat4 inline getInverseRatationMatrixRender();
	mat4 inline getTranslationMatrixRender();
	void inline render();
};
#endif // !CYLINDER_H
