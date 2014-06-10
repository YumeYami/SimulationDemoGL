#include "Rigidbody.h"

using namespace glm;
using namespace std;

Rigidbody::Rigidbody() {
	position = vec4(0, 0, 0, 1);
	velocity = vec4(0, 0, 0, 0);
	mass = 1;
	accuMomentum = vec4(0);

	orientation = vec3(0, 0, 0);
	angularVelocity = vec3(0, 0, 0);
	inertia = vec3(1.0f);
	accuAngularMomentum = vec3(0);

	boundedRadius = 1;

	color = vec4(0, 0, 0, 0);
	transparent = 0.8f;
}
Rigidbody::~Rigidbody() {}

vec3 inline Rigidbody::getMin() {
	return vec3(0, 0, 0);
}
vec3 inline Rigidbody::getMax() {
	return vec3(0, 0, 0);
}
vec3 inline Rigidbody::getSkin() {
	return vec3(1, 1, 1);
}
void inline Rigidbody::addGravity(float gravity) {
	velocity += vec4(0, -gravity, 0, 0);
}
void inline Rigidbody::addMomentum(vec4 momentum) {
	//printVec4("momentum",accuMomentum);
	//accuMomentum += momentum;
	velocity = velocity + (momentum / mass)*0.98f;
	velocity *= 0.90;
	//if ( length(velocity) < 0.1f ) {
	//	velocity = vec4(0);
	//}
}
void inline Rigidbody::addAngularMomentum(vec4 angularMomentum) {
	//angularVelocity += vec3(angularMomentum);
	//if ( length(angularVelocity) >= 1 )angularVelocity *= 1 / length(angularVelocity);
	//printVec4("anguvelo ",vec4(angularVelocity,0));
	accuAngularMomentum += vec3(angularMomentum);
}
void inline Rigidbody::addAngularMomentum(vec3 angularMomentum) {
	//angularVelocity += angularMomentum;
	//if ( length(angularVelocity) >= 1 )angularVelocity *= 1 / length(angularVelocity);
	//printVec4("anguvelo ",vec4(angularVelocity,0));
	accuAngularMomentum += angularMomentum;
}
vec4 inline Rigidbody::getNormal() {
	return normalize(getRotationMatrix()*vec4(0, 1, 0, 0));
}
mat4 inline Rigidbody::getRotationMatrix() {
	return eulerAngleYXZ(orientation.y, orientation.x, orientation.z);
}
mat4 inline Rigidbody::getInverseRatationMatrix() {
	return eulerAngleYXZ(-orientation.y, -orientation.x, -orientation.z);
}
mat4 inline Rigidbody::getTranslationMatrix() {
	return mat4(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				position.x, position.y, position.z, 1.0f);
}
mat4 inline Rigidbody::getRotationMatrixRender() {
	return mat4(1);
}
mat4 inline Rigidbody::getInverseRatationMatrixRender() {
	return mat4(1);
}
mat4 inline Rigidbody::getTranslationMatrixRender() {
	return mat4(1);
}
void inline Rigidbody::updatePosition(vec4 addPosision) {
	position += addPosision;
}
void inline Rigidbody::updatePosition(float time, float gravity) {
	//velocity += (accuMomentum / mass)*0.9f;
	//if ( length(velocity) < 0.25 ) velocity = vec4(0);
	position += (velocity)*time;
	orientation += angularVelocity*time;
	addGravity(gravity);
	if ( gravity != 0 ) velocity *= 0.98f;
	angularVelocity *= 0.98f;
	accuMomentum = vec4(0);
	accuAngularMomentum = vec3(0);
}
void inline Rigidbody::render() {}
void inline Rigidbody::setEdge() {}
void Rigidbody::renderObject(GLuint matrixIDs[], float aTimeStep, float aGravity, int update, int playOneFrame) {
	if ( update || playOneFrame ) {
		updatePosition(aTimeStep, aGravity);
		setEdge();
	}
	glm::mat4 ScaleMatrix = mat4();
	glm::mat4 RotateMatrix = getRotationMatrix();
	glm::mat4 TranslateMatrix = getTranslationMatrix();
	glm::mat4 RotateModel = getRotationMatrixRender();
	glm::mat4 TranslateModel = getTranslationMatrixRender();
	glPushMatrix();
	glUniformMatrix4fv(matrixIDs[0], 1, GL_FALSE, &ScaleMatrix[0][0]);
	glUniformMatrix4fv(matrixIDs[1], 1, GL_FALSE, &RotateMatrix[0][0]);
	glUniformMatrix4fv(matrixIDs[2], 1, GL_FALSE, &TranslateMatrix[0][0]);
	glUniformMatrix4fv(matrixIDs[3], 1, GL_FALSE, &TranslateModel[0][0]);
	glUniformMatrix4fv(matrixIDs[4], 1, GL_FALSE, &RotateModel[0][0]);
	render();
	glPopMatrix();
	color.a = transparent;
}
