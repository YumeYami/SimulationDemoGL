﻿#include "Sphere.h"
#include "Rigidbody.cpp"
class Sphere : public Rigidbody {
public:
	float radius;
	Sphere(vec3 spherePosition, vec3 sphereRotation, vec3 sphereVelocity, float sphereSize, float sphereMass, vec3 sphereColor) {
		position = vec4(spherePosition, 1);
		velocity = (vec4(sphereVelocity, 0));
		mass = sphereMass;

		orientation = sphereRotation;
		angularVelocity = vec3(0, 0, 0);
		inertia = vec3(1.0f);

		radius = sphereSize / 2;
		boundedRadius = radius;
		
		color = vec4(sphereColor, 0.8f);

		//cout<<"newSphere";
	}
	vec3 getSkin() override {
		float skin = 4 * radius*radius;
		return vec3(skin, skin, skin);
	}
	void inline render() {
		GLUquadric* sphere;
		sphere = gluNewQuadric();
		gluQuadricNormals(sphere, GL_SMOOTH);
		glColor4f(color.r, color.g, color.b, color.a);
		gluSphere(sphere, radius, 10, 10);
	}
};
