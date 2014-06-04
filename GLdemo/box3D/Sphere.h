#ifndef SPHERE_H
#define SPHERE_H

#include "Rigidbody.h"

class Sphere : public Rigidbody {
public:
	float radius;

	Sphere(vec3 spherePosition, vec3 sphereRotation, vec3 sphereVelocity, float sphereSize, float sphereMass, vec3 sphereColor);
	~Sphere();

	vec3 inline getSkin();
	void inline render();
};
#endif