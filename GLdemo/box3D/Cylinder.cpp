#include "Cylinder.h"
#include "Rigidbody.cpp"
class Cylinder : public Rigidbody {
public:
	float radius;
	float length;
	Cylinder(vec3 cylinderPosition,vec3 cylinderRotation,vec3 cylinderVelocity,float cylinderRadius,float cylinderLength,float cylinderMass,vec3 cylinderColor){
		mass=cylinderMass;
		position=vec4(cylinderPosition,1);
		velocity=(vec4(cylinderVelocity,0));
		orientation=cylinderRotation;
		radius=cylinderRadius;
		length=cylinderLength;
		color = vec4(cylinderColor,0.8f);

	}
	vec3 getSkin() override {
		float skin = glm::sqrt(4*radius*radius + length*length);
		return vec3(skin,skin,skin);
	}
	vec4 inline getBasePoint(){
		return position;
	}
	vec4 inline getTopPoint(){
		return position + (getNormal()*length);
	}
	mat4 inline getRotationMatrixRender(){
		return eulerAngleYXZ(0.0f,-PI/2,0.0f);
	}
	mat4 inline getInverseRatationMatrixRender(){
		return eulerAngleYXZ(0.0f,PI/2,0.0f);
	}
	mat4 inline getTranslationMatrixRender() {
		vec4 pos = vec4(0,0,-length/2,0);
		return mat4(1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			pos.x,pos.y,pos.z,1.0f);
	}
	void inline render(){
		glColor4f(color.r,color.g,color.b,color.a);
		GLUquadric* cyl;
		cyl=gluNewQuadric();
		gluQuadricNormals(cyl,GL_SMOOTH);
		gluCylinder(cyl,radius,radius,length,20,1);
	}
};