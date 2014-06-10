#include "Cylinder.h"

Cylinder::Cylinder(vec3 cylinderPosition, vec3 cylinderRotation, vec3 cylinderVelocity, float cylinderRadius, float cylinderLength, float cylinderMass, vec3 cylinderColor) {
	position = vec4(cylinderPosition, 1);
	velocity = (vec4(cylinderVelocity, 0));
	mass = cylinderMass;

	orientation = cylinderRotation;
	angularVelocity = vec3(0);
	inertia = vec3(1.0f);

	radius = cylinderRadius;
	length = cylinderLength;
	boundedRadius = glm::sqrt(pow(radius, 2) + pow(length / 2, 2));

	color = vec4(cylinderColor, 0.8f);
}
Cylinder::~Cylinder() {}
vec3 inline Cylinder::getSkin() {
	float skin = glm::sqrt(4 * radius*radius + length*length);
	return vec3(skin, skin, skin);
}
vec4 Cylinder::getBasePoint() {
	return position;
}
vec4 Cylinder::getTopPoint() {
	return position + (getNormal()*length);
}
mat4 inline Cylinder::getRotationMatrixRender() {
	return eulerAngleYXZ(0.0f, -PI / 2, 0.0f);
}
mat4 inline Cylinder::getInverseRatationMatrixRender() {
	return eulerAngleYXZ(0.0f, PI / 2, 0.0f);
}
mat4 inline Cylinder::getTranslationMatrixRender() {
	vec4 pos = vec4(0, 0, -length / 2, 0);
	return mat4(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				pos.x, pos.y, pos.z, 1.0f);
}
void inline Cylinder::render() {
	glColor4f(color.r, color.g, color.b, color.a);
	GLUquadric* cyl;
	cyl = gluNewQuadric();
	gluQuadricNormals(cyl, GL_SMOOTH);
	gluCylinder(cyl, radius, radius, length, 20, 1);
}