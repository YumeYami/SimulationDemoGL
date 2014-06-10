#ifndef CUBE_H
#define CUBE_H

#define cubeVertex0 vec4(-1.0f, -1.0f, -1.0f, 1)
#define cubeVertex1 vec4(-1.0f, -1.0f,  1.0f, 1)
#define cubeVertex2 vec4(-1.0f,  1.0f, -1.0f, 1)
#define cubeVertex3 vec4(-1.0f,  1.0f,  1.0f, 1)
#define cubeVertex4 vec4( 1.0f, -1.0f, -1.0f, 1)
#define cubeVertex5 vec4( 1.0f, -1.0f,  1.0f, 1)
#define cubeVertex6 vec4( 1.0f,  1.0f, -1.0f, 1)
#define cubeVertex7 vec4( 1.0f,  1.0f,  1.0f, 1)

#include "Rigidbody.h"

class Cube : public Rigidbody {
public:
	float vertex[8][3];
	float size;
	vec4* transformVertex;
	vec4* edgeSta;
	vec4* edgeEnd;
	Cube(vec3 cubePosition, vec3 cubeRotation, vec3 cubeVelocity, float cubeSize, float cubeMass, vec3 cubeColor);
	~Cube();
	void inline setEdge();
	vec3 inline getSkin();
	vec4 inline toPosition(int i);
	void inline updateVeloCube(vec4 velo);
	void inline render();
};
#endif