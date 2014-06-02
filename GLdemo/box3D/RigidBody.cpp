//header guard at start of header file
#ifndef OBJECT_H
#define OBJECT_H
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
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
#include <iostream>
#include "Rigidbody.h"
#include "box3DglobalRule.cpp"

using namespace glm;
using namespace std;

class Rigidbody{

public:
	vec4 position;
	vec4 velocity;
	float mass;

	vec3 orientation;
	vec3 angularVelocity;
	float inertia;

	float size;
	float boundedRadius;
	vec4 color;
	float transparent;
	Rigidbody(){
		position = vec4(0, 0, 0, 1);
		mass = 1;

		orientation = vec3(0, 0, 0);
		angularVelocity = vec3(0, 0, 0);
		inertia = 1;
		transparent = 0.8;
		size = 1;
		boundedRadius = 0;
		color = vec4(0, 0, 0, 0);
	}
	virtual vec3 inline getMin(){
		return vec3(0, 0, 0);
	}
	virtual vec3 inline getMax(){
		return vec3(0, 0, 0);
	}
	virtual vec3 inline getSkin(){
		return vec3(1, 1, 1);
	}
	virtual void inline addForce(float gravity){
		velocity += vec4(0, -gravity, 0, 0);
	}
	virtual void inline addMomentum(vec4 momentum){
		velocity = velocity + momentum;
		velocity *= 0.90;
	}
	virtual void inline addAngularMomentum_vec4(vec4 angularMomentum){
		angularVelocity += vec3(angularMomentum);
		if (length(angularVelocity) >= 1)angularVelocity *= 1 / length(angularVelocity);
		//printVec4("anguvelo ",vec4(angularVelocity,0));
	}
	virtual void inline addAngularMomentum(vec3 angularMomentum){
		angularVelocity += angularMomentum;
		if (length(angularVelocity) >= 1)angularVelocity *= 1 / length(angularVelocity);
		//printVec4("anguvelo ",vec4(angularVelocity,0));
	}
	virtual vec4 inline getNormal(){
		return normalize(getRotationMatrix()*vec4(0, 1, 0, 0));
	}
	virtual mat4 inline getRotationMatrix(){
		return eulerAngleYXZ(orientation.y, orientation.x, orientation.z);
	}
	virtual mat4 inline getInverseRatationMatrix(){
		return eulerAngleYXZ(-orientation.y, -orientation.x, -orientation.z);
	}
	//transpose
	virtual mat4 inline getTranslationMatrix(){
		return mat4(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			position.x, position.y, position.z, 1.0f);
	}
	virtual void updatePosition(vec4 addPosision){
		position += addPosision;
	}
	virtual void inline updatePosition(float time, float gravity){
		position += (velocity)*time;
		orientation += angularVelocity*time;
		addForce(gravity);
		if (gravity != 0) velocity *= 0.98f;
		angularVelocity *= 0.98f;
	}
	virtual vec4 getColPoint(){
		return vec4(0, 0, 0, 0);
	}
	void inline printVec4(std::string name, vec4 vec){
		//std::cout<<name<<"\t: [ "<<vec.x<<"\t"<<vec.y<<"\t"<<vec.z<<"\t"<<vec.w<<"\t"<<length(vec)<<" ]\n";
	}
};
//End guard at bottom of header file
#endif 
