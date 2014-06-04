#include "box3DglobalRule.h"
#include "box3DcalculateForce.cpp"

float inline minn(float x, float y);
bool inline convergent_check(Rigidbody* rigid1, Rigidbody* rigid2);
bool inline outOfBound_check(Rigidbody* rigid1, Rigidbody* rigid2);
void inline checkCollision_SphereCube(Sphere* sph1, Cube* cube2);
void inline checkCollision_SphereCylinder(Sphere* sph1, Cylinder* cylinder2);
void inline checkCollision_SpherePlane(Sphere* sph1, Plane* plane2);
void inline checkCollision_SphereSphere(Sphere* sph1, Sphere* sph2);
void inline checkCollision_PlaneCube(Plane* plane1, Cube* cube2);
void inline checkCollision_PlaneCylinder(Plane* plane1, Cylinder* cylinder2);
void inline checkCollision_CubeCube(Cube* cube1, Cube* cube2);
void inline checkCollision_CubeCylinder(Cube* cyl1, Cylinder* cyl2);
void inline checkCollision_CylinderCylinder(Cylinder* cyl1, Cylinder* cyl2);
void inline checkCollision(vector<Cube*> cu, vector<Cylinder*> cy, vector<Plane*> pl, vector<Sphere*> sp);