#ifndef COLLISION_DETECTION_CPP
#define COLLISION_DETECTION_CPP

#include "CollsionGlobalFunc.cpp"
#include "CollisionResponse.cpp"
#include "GridCell.h"
#include "Grid.h"

float inline minn(float x, float y) {
	return (x < y ? x : y);
}

bool inline convergent_check(Rigidbody* rigid1, Rigidbody* rigid2) {
	if ( Plane* plane = dynamic_cast<Plane*>(rigid1) ) {
		return (projectSize(rigid2->velocity, plane->getNormal()) >= 0) ? false : true;
	}
	else  if ( Plane* plane = dynamic_cast<Plane*>(rigid2) ) {
		return (projectSize(rigid1->velocity, plane->getNormal()) >= 0) ? false : true;
	}
	return (dot(rigid1->velocity - rigid2->velocity, rigid1->position - rigid2->position) >= 0) ? false : true;
}
bool inline outOfBound_check(Rigidbody* rigid1, Rigidbody* rigid2) {
	float dist = length(rigid1->position - rigid2->position);
	if ( dist > rigid1->boundedRadius + rigid2->boundedRadius ) return true;
	else if ( !convergent_check(rigid1, rigid2) )return true;
	else return false;
}
//
void inline checkCollision_SphereCube(Sphere* sph1, Cube* cube2) {
	if ( outOfBound_check(sph1, cube2) ) return;
	//bounded sphere check
	for ( int i = 0; i < 12; i++ ) {
		vec4 start = (cube2->edgeSta[i]);
		vec4 end = (cube2->edgeEnd[i]);
		vec4 colPoint = min_dist_segment_to_point(start, end, sph1->position);
		if ( length(colPoint) <= sph1->radius ) {
			colSphere_Cube(sph1, cube2, colPoint);
			return;
		}
	}
}
//
void inline checkCollision_SphereCylinder(Sphere* sph1, Cylinder* cylinder2) {
	if ( outOfBound_check(sph1, cylinder2) ) return;
	//bounded sphere check
	vec4 spherePos = cylinder2->getInverseRatationMatrix()*(sph1->position - cylinder2->position);
	vec4 cylNormal = vec4(0, 1, 0, 0);
	float projectDist = projectSize(spherePos, cylNormal);
	vec4 minDist = projectDist*cylNormal - spherePos;
	if ( length(minDist) >= cylinder2->radius + sph1->radius ) return;
	if ( length(minDist) < cylinder2->radius ) {
		if ( projectDist <= cylinder2->length + sph1->radius ) return;
		else {

			colSphere_Cylinder(sph1, cylinder2, min_dist_segment_to_point(cylinder2->getBasePoint(), cylinder2->getTopPoint(), sph1->position));
		}
	}
	else {
		if ( length(projectDist*cylNormal + vec4(cylinder2->radius, 0, 0, 0) - spherePos) >= sph1->radius ) return;
		else {
			colSphere_Cylinder(sph1, cylinder2, min_dist_segment_to_point(cylinder2->getBasePoint(), cylinder2->getTopPoint(), sph1->position));
		}
	}
}
//
void inline checkCollision_SpherePlane(Sphere* sph1, Plane* plane2) {
	if ( outOfBound_check(sph1, plane2) ) return;
	//bounded sphere check
	vec4 spPos = sph1->position;
	float radius = sph1->radius;
	vec4 centerVec = spPos - plane2->position;
	vec4 height = projectVec(centerVec, plane2->getNormal());
	if ( length(height) <= radius ) {
		colSphere_Plane(sph1, plane2, height);
	}
}
//
void inline checkCollision_SphereSphere(Sphere* sph1, Sphere* sph2) {
	if ( outOfBound_check(sph1, sph2) ) return;
	//bounded sphere check
	vec4 sphPos = sph1->position;
	float radius = sph1->radius;
	vec4 d = sphPos - sph2->position;
	float distance = length(d);
	float sumR = radius + sph2->radius;
	if ( distance <= sumR ) {
		//onCollision
		colSphere_Sphere(sph1, sph2);
	}
}
//some bug
void inline checkCollision_PlaneCube(Plane* plane1, Cube* cube2) {
	if ( outOfBound_check(plane1, cube2) ) return;
	//bounded sphere check
	for ( int i = 0; i < 12; i++ ) {
		vec4 start = (cube2->edgeSta[i]);
		vec4 end = (cube2->edgeEnd[i]);
		if ( dot(start - plane1->position, plane1->getNormal()) <= 0 ) {
			colCube_Plane(cube2, plane1, start - cube2->position);
			return;
		}
		else if ( dot(end - plane1->position, plane1->getNormal()) <= 0 ) {
			colCube_Plane(cube2, plane1, end - cube2->position);
			return;
		}
	}
}
//
void inline checkCollision_PlaneCylinder(Plane* plane1, Cylinder* cylinder2) {
	if ( outOfBound_check(plane1, cylinder2) ) return;
	//bounded sphere check
	vec4 planeNormal = plane1->getNormal();
	if ( projectSize(cylinder2->velocity, planeNormal) >= 0 ) return;
	vec4 dist = cylinder2->position - plane1->position;
	vec4 cylNormal = cylinder2->getNormal();
	vec4 posheight = projectVec(cylinder2->position - plane1->position, planeNormal);
	vec3 temp1 = cross((vec3)planeNormal, (vec3)cylNormal);
	vec4 lowestPos = vec4(cross(temp1, (vec3)cylNormal), 0);
	vec4 bodyheight = projectVec(cylNormal*(cylinder2->length / 2), planeNormal);
	vec4 baseheight = projectVec(lowestPos, planeNormal);
	if ( length(bodyheight) + length(baseheight) >= length(posheight) ) {
		vec4 colPoint(0, 0, 0, 0);

		colPlane_Cylinder(cylinder2, plane1, lowestPos);
	}
}
//on code
void inline checkCollision_CubeCube(Cube* cube1, Cube* cube2) {
	if ( outOfBound_check(cube1, cube2) ) return;
	//bounded sphere check
	//if ( projectSize(sph2->velocity - sph1->velocity, sph2->position - sph1->position) >= 0 ) return;
	vec4 cubePos = cube1->position;
	vec4 d = cubePos - cube2->position;
	float distance = length(d);
	//float sumR = (size + cube2->size*1.4f) / 2;
	if ( distance <= (cube1->boundedRadius + cube2->boundedRadius) / 2.0f ) {
		//onCollision
		colCube_Cube(cube1, cube2, (cube2->position - cube1->position) / 2.0f);
	}
	else return;

	//float size = cube1->size;
	//base is cube1
	//for (int i = 0; i < 12; i++)
	//{
	//	vec4 point0 = inverseRotateCube1*(cube2->edgeSta[i] - cube1->position);
	//	vec4 point1 = inverseRotateCube1*(cube2->edgeEnd[i] - cube1->position);
	//	//point_ModelCube1
	//	vec4 line;
	//	vec4 startline;
	//	vec4 endline;
	//	vec4 proj;

	//	//left
	//	if(point1.x > point0.x){
	//		line = point1 - point0;
	//		startline = point0;
	//		endline = point1;
	//	}
	//	else
	//	{
	//		line = point0 - point1;
	//		startline = point1;
	//		endline = point0;
	//	}
	//	if(startline.x<=size && endline.x>=size){
	//		proj = line * abs( (size - abs(startline.x)) / line.x);
	//		if(abs(startline.y + proj.y)<=size/2 && abs(startline.z + proj.z)<=size/2) {
	//			colCube_Cube(cube1,cube2,startline+proj);
	//			return;
	//		}
	//	}

	//	//right
	//	if(point1.x < point0.x){
	//		line = point1 - point0;
	//		startline = point0;
	//		endline = point1;
	//	}
	//	else
	//	{
	//		line = point0 - point1;
	//		startline = point1;
	//		endline = point0;
	//	}
	//	if(startline.x>=-size && endline.x<=-size){
	//		proj = line * abs( (size - abs(startline.x)) / line.x);
	//		if(abs(startline.y + proj.y) <= size/2 && abs(startline.z + proj.z)<=size/2) {
	//			colCube_Cube(cube1,cube2,startline+proj);
	//			return;
	//		}
	//	}

	//	//front
	//	if(point1.z > point0.z){
	//		line = point1 - point0;
	//		startline = point0;
	//		endline = point1;
	//	}
	//	else
	//	{
	//		line = point0 - point1;
	//		startline = point1;
	//		endline = point0;
	//	}
	//	if(startline.z<=size && endline.z>=size){
	//		proj = line * abs( (size - abs(startline.z)) / line.z);
	//		if(abs(startline.y + proj.y)<=size/2 && abs(startline.x + proj.x)<=size/2) {
	//			colCube_Cube(cube1,cube2,startline+proj);
	//			return;
	//		}
	//	}

	//	//back
	//	if(point1.z < point0.z){
	//		line = point1 - point0;
	//		startline = point0;
	//		endline = point1;
	//	}
	//	else
	//	{
	//		line = point0 - point1;
	//		startline = point1;
	//		endline = point0;
	//	}
	//	if(startline.z>=-size && endline.z<=-size){
	//		proj = line * abs( (size - abs(startline.z)) / line.z);
	//		if(abs(startline.y + proj.y) <= size/2 && abs(startline.x + proj.x)<=size/2) {
	//			colCube_Cube(cube1,cube2,startline+proj);
	//			return;
	//		}
	//	}

	//	//top
	//	if(point1.y > point0.y){
	//		line = point1 - point0;
	//		startline = point0;
	//		endline = point1;
	//	}
	//	else
	//	{
	//		line = point0 - point1;
	//		startline = point1;
	//		endline = point0;
	//	}
	//	if(startline.y<=size && endline.y>=size){
	//		proj = line * abs( (size - abs(startline.y)) / line.y);
	//		if(abs(startline.x + proj.x)<=size/2 && abs(startline.z + proj.z)<=size/2) {
	//			colCube_Cube(cube1,cube2,startline+proj);
	//			return;
	//		}
	//	}

	//	//right
	//	if(point1.y < point0.y){
	//		line = point1 - point0;
	//		startline = point0;
	//		endline = point1;
	//	}
	//	else
	//	{
	//		line = point0 - point1;
	//		startline = point1;
	//		endline = point0;
	//	}
	//	if(startline.y>=-size && endline.y<=-size){
	//		proj = line * abs( (size - abs(startline.z)) / line.z);
	//		if(abs(startline.x + proj.x) <= size/2 && abs(startline.z + proj.z)<=size/2) {
	//			colCube_Cube(cube1,cube2,startline+proj);
	//			return;
	//		}
	//	}
	//}
}
//
void inline checkCollision_CubeCylinder(Cube* cyl1, Cylinder* cyl2) {
	if ( projectSize(cyl2->velocity - cyl1->velocity, cyl2->position - cyl1->position) >= 0 ) return;
	if ( projectSize(cyl2->velocity - cyl1->velocity, cyl2->position - cyl1->position) >= 0 ) return;
	vec4 sphPos = cyl1->position;
	float radius = cyl1->size;
	vec4 d = sphPos - cyl2->position;
	float distance = length(d);
	float sumR = radius + cyl2->radius;
	if ( distance <= sumR ) {
		//onCollision
		colCube_Cylinder(cyl1, cyl2, (cyl2->position - cyl1->position));
	}
}

//float f1(float t,float r0,float r1,float h1b1Div2, float c1sqr,float a2,float b2){
//	float omt = 1-t;
//	float tsqr =t*t;
//	float omtsqr = omt*omt;
//	float term0=r0*sqrt(omtsqr+tsqr);
//	float term1=r1*sqrt(omtsqr+c1sqr*tsqr);
//	float term2 = h1b1Div2;
//	float term3 = abs(omt*a2+t*b2);
//	return term0+term1+term2-term3;
//}
//float fDer1(float t,float r0,float r1,float h1b1Div2, float c1sqr,float a2,float b2){
//	float omt = 1-t;
//	float tsqr =t*t;
//	float omtsqr = omt*omt;
//	float term0=r0*(2*t-1)/sqrt(omtsqr+tsqr);
//	float term1=r1*((1+c1sqr)*t-1)/sqrt(omtsqr+c1sqr*tsqr);
//	float term2 = h1b1Div2;
//	float term3 = (b2-a2)*sign(omt*a2+t*b2);
//	return term0+term1+term2-term3;
//}
//
//int separatedByCylinderPerpendiculars2(vec3 p0,vec3 w0, float r0, float h0, vec3 p1,vec3 w1,float r1,float h1){
//	vec3 delta = p1-p0;
//	float c1 = dot(w0,w1);
//	float b1 = sqrt(1-c1*c1);
//	vec3 v0 = (w1-c1*w0)/b1;
//	vec3 u0 = cross(v0,w0);
//	float a2 = dot(delta,u0);
//	float b2 = dot(delta,v0);
////	(float t,float r0,float r1,float h1b1Div2, float c1sqr,float a2,float b2)
//	if(f1(0,r0,r1,h1*b1/2,c1*c1,a2,b2)<=0||f1(1,r0,r1,h1*b1/2,c1*c1,a2,b2)<=0) return 1;
//	if(fDer1(0,r0,r1,h1*b1/2,c1*c1,a2,b2)>=0||fDer1(1,r0,r1,h1*b1/2,c1*c1,a2,b2)<=0) return 0;
//
//	float t0,t1,fd0,fd1,tmid,fdmid;
//	int i;
//	t0=0;
//	t1=1;
//	tmid = 0.5*(t0+t1);
//	if(f1(tmid,r0,r1,h1*b1/2,c1*c1,a2,b2)<=0) return 1;
//	
//	a2=-a2;
//	if(f1(0,r0,r1,h1*b1/2,c1*c1,a2,b2)<=0 || f1(1,r0,r1,h1*b1/2,c1*c1,a2,b2)<=0) return 1;
//	if(fDer1(0,r0,r1,h1*b1/2,c1*c1,a2,b2)>=0 || fDer1(1,r0,r1,h1*b1/2,c1*c1,a2,b2)<=0) return 0;
//	
//	if(f1(tmid,r0,r1,h1*b1/2,c1*c1,a2,b2)<=0) return 1;
//	return 1;
//}
//
//float g1(float s, float t, float r0, float h0Div2, float r1,float h1Div2,
//		float a0,float b0,float c0,float a1,float b1,float c1,float lenDelta){
//			float omsmt = 1-s-t;
//			float ssqr =s*s;
//			float tsqr=t*t;
//			float omsmtsqr = omsmt*omsmt;
//			float temp=ssqr+tsqr+omsmtsqr;
//			float L0=a0*s+b0*t+c0*omsmt;
//			float L1 = a1*s+b1*t+c1*omsmt;
//			float Q0 = temp-L0*L0;
//			float Q1=temp-L1*L1;
//			return r0*sqrt(Q0)+r1*sqrt(Q1)+h0Div2*abs(L0)+h1Div2*abs(L1)-omsmt*lenDelta;
//}
//vec2 gDer1(float s, float t, float r0, float h0Div2, float r1,float h1Div2,
//		float a0,float b0,float c0,float a1,float b1,float c1,float lenDelta){
//			float omsmt = 1-s-t;
//			float ssqr =s*s;
//			float tsqr=t*t;
//			float omsmtsqr = omsmt*omsmt;
//			float temp=ssqr+tsqr+omsmtsqr;
//			float L0=a0*s+b0*t+c0*omsmt;
//			float L1 = a1*s+b1*t+c1*omsmt;
//			float Q0 = temp-L0*L0;
//			float Q1=temp-L1*L1;
//			float diffS = s-omsmt, diffT = t-omsmt;
//			float diffa0c0 = a0-c0,diffa1c1=a1-c1,diffb0c0=b0-c0,diffb1c1=b1-c1;
//			float halfQ0s = diffS-diffa0c0*L0, halfQ1s=diffS-diffa1c1*L1;
//			float halfQ0t = diffT-diffa0c0*L0, halfQ1t=diffT-diffa1c1*L1;
//			float factor0 = r0/sqrt(Q0),factor1=r1/sqrt(L1);
//			float signL0 = sign(L0),signL1=sign(L1);
//			vec2 gradient = vec2(0,0);
//			gradient[0]+=halfQ0s*factor0;
//			gradient[0]+=halfQ1s*factor1;
//			gradient[0]+=h0Div2*diffa0c0*signL0;
//			gradient[0]+=h1Div2*diffa1c1*signL1;
//			gradient[0]+=lenDelta;
//			gradient[1]+=halfQ0t*factor0;
//			gradient[1]+=halfQ1t*factor1;
//			gradient[1]+=h0Div2*diffb0c0*signL0;
//			gradient[1]+=h1Div2*diffb1c1*signL1;
//			gradient[1]+=lenDelta;
//			return gradient;
//}
//int separatedByOtherDirections1(vec3 w0,float r0,float h0,vec3 w1,float r1,float h1,vec3 delta){
//	return 1;
//}
void inline checkCollision_CylinderCylinder(Cylinder* cyl1, Cylinder* cyl2) {
	if ( projectSize(cyl2->velocity - cyl1->velocity, cyl2->position - cyl1->position) >= 0 ) return;
	vec4 minDistSegment = min_dist_segment_to_segment(cyl1->getBasePoint(), cyl1->getTopPoint(), cyl2->getBasePoint(), cyl2->getTopPoint());
	float minDistLine = min_dist_line_to_line(cyl1->getBasePoint(), cyl1->getTopPoint(), cyl2->getBasePoint(), cyl2->getTopPoint());
	if ( length(minDistSegment) >= cyl1->radius + cyl2->radius )return;
	if ( minDistLine == length(minDistSegment) ) {
		colCylinder_Cylinder(cyl1, cyl2, minDistSegment);
		return;
	}
	vec4 sphPos = cyl1->position;
	float radius = cyl1->size;
	vec4 d = sphPos - cyl2->position;
	float distance = length(d);
	float sumR = (radius + cyl2->radius) / 2;
	if ( distance <= sumR ) {
		//onCollision
		colCylinder_Cylinder(cyl1, cyl2, (cyl2->position - cyl1->position));
	}
	/*if(projectSize(cylinder2->getVelocity() - cylinder1->getVelocity(),cylinder2->getPosition() - cylinder1->getPosition()) >= 0) return;
	vec4 minimumDist = dist3D_Segment_to_Segment(cylinder1->getEndPoint1(),cylinder1->getEndPoint2(),cylinder2->getEndPoint1(),cylinder2->getEndPoint2());

	//dummy
	void inline checkCollision_CylinderCylinder(Cylinder* cylinder1,Cylinder* cylinder2){
	if(projectSize(cylinder2->velocity - cylinder1->velocity,cylinder2->position - cylinder1->position) >= 0) return;
	vec4 minimumDist = dist3D_Segment_to_Segment(cylinder1->getBasePoint(),cylinder1->getTopPoint(),cylinder2->getBasePoint(),cylinder2->getTopPoint());
	if(length(minimumDist)>= cylinder1->radius + cylinder2->radius) return;
	else if(length(minimumDist) <= cylinder1->radius) colCylinder_Cylinder(cylinder1,cylinder2);
	else{

	//}*/
	//vec3 c0 = (vec3)cy1->position;
	//vec3 c1 = (vec3)cy2->position;
	//vec3 w0=(vec3)cy1->getNormal();
	//vec3 w1=(vec3)cy2->getNormal();
	//float r0 = cy1->radius;
	//float r1 = cy2->radius;
	//float h0 = cy1->length;
	//float h1 = cy2->length;
	//vec3 delta = c0 - c1;
	//vec3 W0xW1 = cross(w0,w1);
	//float lenW0xW1 = length(W0xW1);
	//float h0Div2 = h0/2;
	//float h1Div2 = h1/2;
	//float rSum = r0+r1;
	//if(lenW0xW1>0){
	//	//by w0
	//	if(r1*lenW0xW1+h0Div2+h1Div2*abs(dot(w0,w1))-abs(dot(w0,delta))) return;
	//	//by w1
	//	if(r0*lenW0xW1+h0Div2*abs(dot(w0,w1))+h1Div2-abs(dot(w0,delta))) return;
	//	//by w0xw1
	//	if(rSum*lenW0xW1-abs(dot(W0xW1,delta))<0) return;
	//	//if(separatedByCylinderPerpendiculars2(c0,w0,r0,h0,c1,w1,r1,h1)) return;
	//	//if(separatedByCylinderPerpendiculars2(c1,w1,r1,h1,c0,w0,r0,h0)) return;
	//	//if(separatedByOtherDirections1(w0,r0,h0,w1,r1,h1,delta)) return;
	//} else {
	//	//by height
	//	if(h0Div2+h1Div2-abs(dot(w0,delta))<0) return;
	//	//by radius	
	//	if(rSum-length((delta-dot(w0,delta)*w0))<0) return;
	//}
	//colCylinder_Cylinder(cy1,cy2);
}

void inline checkCollision(vector<Cube*> cu, vector<Cylinder*> cy, vector<Plane*> pl, vector<Sphere*> sp) {
	//cout <<"size "<<cu.size()<<" "<<cy.size()<<" "<<pl.size()<<" "<<sp.size()<<"\n";
	for ( int i = 0; i < sp.size(); i++ ) {
		Sphere* sp1 = sp.at(i);
		for ( int j = 0; j < cu.size(); j++ ) checkCollision_SphereCube(sp1, cu.at(j));
		for ( int j = 0; j < cy.size(); j++ ) checkCollision_SphereCylinder(sp1, cy.at(j));
		for ( int j = 0; j < pl.size(); j++ ) checkCollision_SpherePlane(sp1, pl.at(j));
		if ( i < sp.size() - 1 ) {
			for ( int j = i + 1; j < sp.size(); j++ ) checkCollision_SphereSphere(sp1, sp.at(j));
		}
	}
	for ( int i = 0; i < pl.size(); i++ ) {
		Plane* pl1 = pl.at(i);
		for ( int j = 0; j < cu.size(); j++ ) checkCollision_PlaneCube(pl1, cu.at(j));
		for ( int j = 0; j < cy.size(); j++ ) checkCollision_PlaneCylinder(pl1, cy.at(j));
	}
	for ( int i = 0; i < cu.size(); i++ ) {
		Cube* cu1 = cu.at(i);
		if ( i < cu.size() - 1 ) for ( int j = i + 1; j < cu.size(); j++ ) checkCollision_CubeCube(cu1, cu.at(j));
		for ( int j = 0; j < cy.size(); j++ ) checkCollision_CubeCylinder(cu1, cy.at(j));
	}
	for ( int i = 0; i < cy.size(); i++ ) {
		Cylinder* cy1 = cy.at(i);
		if ( i < cy.size() - 1 )
		for ( int j = i + 1; j < cy.size(); j++ )
			checkCollision_CylinderCylinder(cy1, cy.at(j));

	}
}
#endif // !COLLISION_DETECTION_CPP