#include "Plane.h"

Plane::Plane(vec3 planePosition, vec3 planeRotation, vec3 planeVelocity, float planeSize, float planeMass, vec3 planeColor) {
	position = vec4(planePosition, 1);
	velocity = vec4(planeVelocity, 0);
	mass = planeMass;

	orientation = planeRotation;
	angularVelocity = vec3(0);
	inertia = vec3(1.0f);

	size = planeSize;
	boundedRadius = pow(2.0f,0.5f)*size;

	p0[0] = planeVertex0.x*size / 2; p0[1] = planeVertex0.y*size / 2; p0[2] = planeVertex0.z*size / 2;
	p1[0] = planeVertex1.x*size / 2; p1[1] = planeVertex1.y*size / 2; p1[2] = planeVertex1.z*size / 2;
	p2[0] = planeVertex2.x*size / 2; p2[1] = planeVertex2.y*size / 2; p2[2] = planeVertex2.z*size / 2;
	p3[0] = planeVertex3.x*size / 2; p3[1] = planeVertex3.y*size / 2; p3[2] = planeVertex3.z*size / 2;

	color = vec4(planeColor, 0.8f);
}
Plane::~Plane() {}
void inline Plane::render() {
	glBegin(GL_QUADS); {
		//back
		glColor4f(color.r, color.g, color.b, color.a);
		glVertex3fv(p0);
		glVertex3fv(p1);
		glVertex3fv(p2);
		glVertex3fv(p3);
	}glEnd();
}
