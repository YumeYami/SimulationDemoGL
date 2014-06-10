#ifndef RIGIDBODY_H
#define RIGIDBODY_H

// Include standard headers
#include <cstdio>
#include <iostream>
#include <string>
//#include <stdlib.h>
#include <vector>
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GL/glfw.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

// Include AntTweakBar
//#include <AntTweakBar.h>
//#include <common/shader.hpp>
//#include <common/texture.hpp>
//#include <common/controls.hpp>
//#include <common/objloader.hpp>
//#include <common/vboindexer.hpp>
//#include <common/quaternion_utils.hpp> // See quaternion_utils.cpp for RotationBetweenVectors, LookAt and RotateTowards
#include "CollisionGlobalFunc.cpp"
#include "util_func.h"

void inline printVec4(std::string name, vec4 vec);

class Rigidbody {

public:
	vec4 position;
	vec4 velocity;
	float mass;
	vec4 accuMomentum;

	vec3 orientation;
	vec3 angularVelocity;
	vec3 inertia;
	vec3 accuAngularMomentum;

	float boundedRadius;

	vec4 color;
	float transparent;

	Rigidbody();
	~Rigidbody();
	virtual vec3 inline getMin();
	virtual vec3 inline getMax();
	virtual vec3 inline getSkin();
	virtual void inline addGravity(float gravity);
	virtual void inline addMomentum(vec4 momentum);
	virtual void inline addAngularMomentum(vec4 angularMomentum);
	virtual void inline addAngularMomentum(vec3 angularMomentum);
	virtual vec4 inline getNormal();
	virtual mat4 inline getRotationMatrix();
	virtual mat4 inline getInverseRatationMatrix();
	virtual mat4 inline getTranslationMatrix();
	virtual void inline updatePosition(vec4 addPosision);
	virtual void inline updatePosition(float time, float gravity);
	virtual void inline render() = 0;
	virtual void inline setEdge();
	void renderObject(GLuint matrixIDs[], float aTimeStep, float aGravity, int update, int playOneFrame);
	//void inline renderObject();
};
#endif