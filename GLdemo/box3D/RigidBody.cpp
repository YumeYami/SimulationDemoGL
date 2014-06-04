//header guard at start of header file
#ifndef OBJECT_H
#define OBJECT_H
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
#include "Rigidbody.h"
#include "box3DglobalRule.cpp"
#include "util_func.h"

using namespace glm;
using namespace std;


void inline printVec4(std::string name, vec4 vec) {
	cout << name << "\t: [ " << vec.x << "\t" << vec.y << "\t" << vec.z << "\t" << vec.w << "\t" << length(vec) << " ]\n";
}

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

	float size;
	float boundedRadius;

	vec4 color;
	float transparent;

	Rigidbody() {
		position = vec4(0, 0, 0, 1);
		velocity = vec4(0, 0, 0, 0);
		mass = 1;
		accuMomentum = vec4(0);

		orientation = vec3(0, 0, 0);
		angularVelocity = vec3(0, 0, 0);
		inertia = vec3(1.0f);
		accuAngularMomentum = vec3(0);

		size = 1;
		boundedRadius = 1;

		color = vec4(0, 0, 0, 0);
		transparent = 0.8f;
	}
	virtual vec3 inline getMin() {
		return vec3(0, 0, 0);
	}
	virtual vec3 inline getMax() {
		return vec3(0, 0, 0);
	}
	virtual vec3 inline getSkin() {
		return vec3(1, 1, 1);
	}
	virtual void inline addGravity(float gravity) {
		velocity += vec4(0, -gravity, 0, 0);
	}
	virtual void inline addMomentum(vec4 momentum) {
		//printVec4("momentum",accuMomentum);
		//accuMomentum += momentum;
		velocity = velocity + momentum / mass;
		velocity *= 0.90;
		if ( length(velocity) < 0.1f ) {
			velocity = vec4(0);
		}
	}
	virtual void inline addAngularMomentum(vec4 angularMomentum) {
		//angularVelocity += vec3(angularMomentum);
		//if ( length(angularVelocity) >= 1 )angularVelocity *= 1 / length(angularVelocity);
		//printVec4("anguvelo ",vec4(angularVelocity,0));
		accuAngularMomentum += vec3(angularMomentum);
	}
	virtual void inline addAngularMomentum(vec3 angularMomentum) {
		//angularVelocity += angularMomentum;
		//if ( length(angularVelocity) >= 1 )angularVelocity *= 1 / length(angularVelocity);
		//printVec4("anguvelo ",vec4(angularVelocity,0));
		accuAngularMomentum += angularMomentum;
	}
	virtual vec4 inline getNormal() {
		return normalize(getRotationMatrix()*vec4(0, 1, 0, 0));
	}
	virtual mat4 inline getRotationMatrix() {
		return eulerAngleYXZ(orientation.y, orientation.x, orientation.z);
	}
	virtual mat4 inline getInverseRatationMatrix() {
		return eulerAngleYXZ(-orientation.y, -orientation.x, -orientation.z);
	}
	//transpose
	virtual mat4 inline getTranslationMatrix() {
		return mat4(1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					position.x, position.y, position.z, 1.0f);
	}
	virtual void inline updatePosition(vec4 addPosision) {
		position += addPosision;
	}
	virtual void inline updatePosition(float time, float gravity) {
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
	virtual void inline render() = 0;
	//virtual void inline setEdge();
};
//End guard at bottom of header file
#endif 
