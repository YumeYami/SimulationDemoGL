#include "Cube.h"
#include "Rigidbody.cpp"
class Cube : public Rigidbody{
public:
	float maxRadius;
	float vertex[8][3];
	float tranformVertex[8][3];
	vec4* edgeSta;
	vec4* edgeEnd;
	Cube(vec3 cubePosition, vec3 cubeRotation, vec3 cubeVelocity, float cubeSize, float cubeMass, vec3 cubeColor) {
		orientation = cubeRotation;
		position = vec4(cubePosition, 1);
		velocity = (vec4(cubeVelocity, 0));
		angularVelocity = vec3(0, 0, 0);
		size = cubeSize;
		mass = cubeMass;
		color = vec4(cubeColor, 0.8f);
		inertia = 1;
		maxRadius = size*glm::sqrt(3.0f)*0.5;

		vertex[0][0] = cubeVertex0.x*size / 2; vertex[0][1] = cubeVertex0.y*size / 2; vertex[0][2] = cubeVertex0.z*size / 2;
		vertex[1][0] = cubeVertex1.x*size / 2; vertex[1][1] = cubeVertex1.y*size / 2; vertex[1][2] = cubeVertex1.z*size / 2;
		vertex[2][0] = cubeVertex2.x*size / 2; vertex[2][1] = cubeVertex2.y*size / 2; vertex[2][2] = cubeVertex2.z*size / 2;
		vertex[3][0] = cubeVertex3.x*size / 2; vertex[3][1] = cubeVertex3.y*size / 2; vertex[3][2] = cubeVertex3.z*size / 2;
		vertex[4][0] = cubeVertex4.x*size / 2; vertex[4][1] = cubeVertex4.y*size / 2; vertex[4][2] = cubeVertex4.z*size / 2;
		vertex[5][0] = cubeVertex5.x*size / 2; vertex[5][1] = cubeVertex5.y*size / 2; vertex[5][2] = cubeVertex5.z*size / 2;
		vertex[6][0] = cubeVertex6.x*size / 2; vertex[6][1] = cubeVertex6.y*size / 2; vertex[6][2] = cubeVertex6.z*size / 2;
		vertex[7][0] = cubeVertex7.x*size / 2; vertex[7][1] = cubeVertex7.y*size / 2; vertex[7][2] = cubeVertex7.z*size / 2;
		//setEdge(&edgeStart,&edgeEnd,vertex);
		edgeSta = new vec4[12];
		edgeEnd = new vec4[12];
		setEdge();

	}
	void inline setEdge(){
		vec4 temp0 = position + getRotationMatrix()*vec4(vertex[0][0], vertex[0][1], vertex[0][2], 0);
		vec4 temp1 = position + getRotationMatrix()*vec4(vertex[1][0], vertex[1][1], vertex[1][2], 0);
		vec4 temp2 = position + getRotationMatrix()*vec4(vertex[2][0], vertex[2][1], vertex[2][2], 0);
		vec4 temp3 = position + getRotationMatrix()*vec4(vertex[3][0], vertex[3][1], vertex[3][2], 0);
		vec4 temp4 = position + getRotationMatrix()*vec4(vertex[4][0], vertex[4][1], vertex[4][2], 0);
		vec4 temp5 = position + getRotationMatrix()*vec4(vertex[5][0], vertex[5][1], vertex[5][2], 0);
		vec4 temp6 = position + getRotationMatrix()*vec4(vertex[6][0], vertex[6][1], vertex[6][2], 0);
		vec4 temp7 = position + getRotationMatrix()*vec4(vertex[7][0], vertex[7][1], vertex[7][2], 0);
		edgeSta[0] = temp0;
		edgeEnd[0] = temp1;
		edgeSta[1] = temp0;
		edgeEnd[1] = temp2;
		edgeSta[2] = temp1;
		edgeEnd[2] = temp3;
		edgeSta[3] = temp2;
		edgeEnd[3] = temp3;
		edgeSta[4] = temp4;
		edgeEnd[4] = temp5;
		edgeSta[5] = temp4;
		edgeEnd[5] = temp6;
		edgeSta[6] = temp5;
		edgeEnd[6] = temp7;
		edgeSta[7] = temp6;
		edgeEnd[7] = temp7;
		edgeSta[8] = temp0;
		edgeEnd[8] = temp4;
		edgeSta[9] = temp1;
		edgeEnd[9] = temp5;
		edgeSta[10] = temp2;
		edgeEnd[10] = temp6;
		edgeSta[11] = temp3;
		edgeEnd[11] = temp7;
	}
	vec3 getSkin() override {
		return vec3(size, size, size)*glm::sqrt(2.0f);
	}
	vec4 inline toPosition(int i){
		return vec4(vertex[i][0], vertex[i][1], vertex[i][2], 1);
	}
	void inline updateVeloCube(vec4 velo){
		addMomentum(velo);
	}
	void inline render(){
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
};
