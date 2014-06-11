#include "Cube.h"
#include "Rigidbody.h"
Cube::Cube(vec3 cubePosition, vec3 cubeRotation, vec3 cubeVelocity, float cubeSize, float cubeMass, vec3 cubeColor) {
	position = vec4(cubePosition, 1);
	velocity = (vec4(cubeVelocity, 0));
	mass = cubeMass;

	orientation = cubeRotation;
	angularVelocity = vec3(0, 0, 0);
	inertia = vec3(1.0f);

	size = cubeSize/2;
	color = vec4(cubeColor, 0.6f);
	boundedRadius = size*glm::sqrt(3.0f);

	vertex[0][0] = cubeVertex0.x*size; vertex[0][1] = cubeVertex0.y*size; vertex[0][2] = cubeVertex0.z*size;
	vertex[1][0] = cubeVertex1.x*size; vertex[1][1] = cubeVertex1.y*size; vertex[1][2] = cubeVertex1.z*size;
	vertex[2][0] = cubeVertex2.x*size; vertex[2][1] = cubeVertex2.y*size; vertex[2][2] = cubeVertex2.z*size;
	vertex[3][0] = cubeVertex3.x*size; vertex[3][1] = cubeVertex3.y*size; vertex[3][2] = cubeVertex3.z*size;
	vertex[4][0] = cubeVertex4.x*size; vertex[4][1] = cubeVertex4.y*size; vertex[4][2] = cubeVertex4.z*size;
	vertex[5][0] = cubeVertex5.x*size; vertex[5][1] = cubeVertex5.y*size; vertex[5][2] = cubeVertex5.z*size;
	vertex[6][0] = cubeVertex6.x*size; vertex[6][1] = cubeVertex6.y*size; vertex[6][2] = cubeVertex6.z*size;
	vertex[7][0] = cubeVertex7.x*size; vertex[7][1] = cubeVertex7.y*size; vertex[7][2] = cubeVertex7.z*size;
	//setEdge(&edgeStart,&edgeEnd,vertex);
	edgeSta = new vec4[12];
	edgeEnd = new vec4[12];
	transformVertex = new vec4[8];
	setEdge();
}
Cube::~Cube() {}
void inline Cube::setEdge() {
	transformVertex[0] = position + getRotationMatrix()*vec4(vertex[0][0], vertex[0][1], vertex[0][2], 0);
	transformVertex[1] = position + getRotationMatrix()*vec4(vertex[1][0], vertex[1][1], vertex[1][2], 0);
	transformVertex[2] = position + getRotationMatrix()*vec4(vertex[2][0], vertex[2][1], vertex[2][2], 0);
	transformVertex[3] = position + getRotationMatrix()*vec4(vertex[3][0], vertex[3][1], vertex[3][2], 0);
	transformVertex[4] = position + getRotationMatrix()*vec4(vertex[4][0], vertex[4][1], vertex[4][2], 0);
	transformVertex[5] = position + getRotationMatrix()*vec4(vertex[5][0], vertex[5][1], vertex[5][2], 0);
	transformVertex[6] = position + getRotationMatrix()*vec4(vertex[6][0], vertex[6][1], vertex[6][2], 0);
	transformVertex[7] = position + getRotationMatrix()*vec4(vertex[7][0], vertex[7][1], vertex[7][2], 0);
	edgeSta[0] = transformVertex[0];
	edgeEnd[0] = transformVertex[1];
	edgeSta[1] = transformVertex[0];
	edgeEnd[1] = transformVertex[2];
	edgeSta[2] = transformVertex[1];
	edgeEnd[2] = transformVertex[3];
	edgeSta[3] = transformVertex[2];
	edgeEnd[3] = transformVertex[3];
	edgeSta[4] = transformVertex[4];
	edgeEnd[4] = transformVertex[5];
	edgeSta[5] = transformVertex[4];
	edgeEnd[5] = transformVertex[6];
	edgeSta[6] = transformVertex[5];
	edgeEnd[6] = transformVertex[7];
	edgeSta[7] = transformVertex[6];
	edgeEnd[7] = transformVertex[7];
	edgeSta[8] = transformVertex[0];
	edgeEnd[8] = transformVertex[4];
	edgeSta[9] = transformVertex[1];
	edgeEnd[9] = transformVertex[5];
	edgeSta[10] = transformVertex[2];
	edgeEnd[10] = transformVertex[6];
	edgeSta[11] = transformVertex[3];
	edgeEnd[11] = transformVertex[7];
}
vec3 inline Cube::getSkin()  {
	return vec3(size, size, size)*glm::sqrt(2.0f);
}
vec4 inline Cube::toPosition(int i) {
	return vec4(vertex[i][0], vertex[i][1], vertex[i][2], 1);
}
void inline Cube::updateVeloCube(vec4 velo) {
	addMomentum(velo);
}
void inline Cube::render() {
	glBegin(GL_QUADS); {
		//back
		//glColor3f(1,1,0);
		glColor4f(color.r, color.g, color.b, color.a);
		glVertex3fv(vertex[0]);
		glVertex3fv(vertex[1]);
		glVertex3fv(vertex[3]);
		glVertex3fv(vertex[2]);
		//left
		//glColor3f(0,1,0);
		//glColor3f(color.r,color.g,color.b);
		glVertex3fv(vertex[4]);
		glVertex3fv(vertex[0]);
		glVertex3fv(vertex[2]);
		glVertex3fv(vertex[6]);
		//right
		//glColor3f(1,0,1);
		//glColor3f(color.r,color.g,color.b);
		glVertex3fv(vertex[5]);
		glVertex3fv(vertex[1]);
		glVertex3fv(vertex[3]);
		glVertex3fv(vertex[7]);
		//up
		//glColor3f(1,0,0);
		//glColor3f(color.r,color.g,color.b);
		glVertex3fv(vertex[6]);
		glVertex3fv(vertex[7]);
		glVertex3fv(vertex[3]);
		glVertex3fv(vertex[2]);
		//down
		//glColor3f(0,1,1);
		//glColor3f(color.r,color.g,color.b);
		glVertex3fv(vertex[0]);
		glVertex3fv(vertex[1]);
		glVertex3fv(vertex[5]);
		glVertex3fv(vertex[4]);
		//front
		//glColor3f(0,1,1);
		//glColor3f(color.r,color.g,color.b);
		glVertex3fv(vertex[4]);
		glVertex3fv(vertex[5]);
		glVertex3fv(vertex[7]);
		glVertex3fv(vertex[6]);

	}glEnd();
}