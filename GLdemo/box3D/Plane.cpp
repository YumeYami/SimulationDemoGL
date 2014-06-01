#include "Plane.h"
#include "Rigidbody.cpp"
using namespace std;
class Plane : public Rigidbody {
public:
	float p0[3];
	float p1[3];
	float p2[3];
	float p3[3];
	Plane(vec3 planePosition,vec3 planeRotation,vec3 planeVelocity,float planeSize,float planeMass,vec3 planeColor){
		size=planeSize;
		mass=planeMass;
		color=vec4(planeColor,0.8f);
		angularVelocity=vec3(0,0,0);
		p0[0] = planeVertex0.x*size/2; p0[1] = planeVertex0.y*size/2; p0[2] = planeVertex0.z*size/2;
		p1[0] = planeVertex1.x*size/2; p1[1] = planeVertex1.y*size/2; p1[2] = planeVertex1.z*size/2;
		p2[0] = planeVertex2.x*size/2; p2[1] = planeVertex2.y*size/2; p2[2] = planeVertex2.z*size/2;
		p3[0] = planeVertex3.x*size/2; p3[1] = planeVertex3.y*size/2; p3[2] = planeVertex3.z*size/2;
		orientation=planeRotation;
		position=vec4(planePosition,1);
		velocity=(vec4(planeVelocity,0));
	}
	void inline render(){
		glBegin(GL_QUADS);{
			//back
			glColor4f(color.r,color.g,color.b,color.a);
			glVertex3fv(p0);
			glVertex3fv(p1);
			glVertex3fv(p2);
			glVertex3fv(p3);
		}glEnd();
	}
};
