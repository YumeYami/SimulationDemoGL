#ifndef COLLISION_DETECTION_CPP
#define COLLISION_DETECTION_CPP

#include "CollisionGlobalFunc.cpp"
#include "CollisionResponse.cpp"
#include "GridCell.h"
#include "Grid.h"

float inline minn(float x, float y) {
	return (x < y ? x : y);
}
void inline printv4(string name, vec4 &vec) {
	cout << name << " :[" << vec.x << " " << vec.y << " " << vec.z << " " << length(vec) << "\n";
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
bool inline checkCollision_ParticleCube(vec4 point, Cube* cube) {
	vec4 particle_CubeSpace = cube->getInverseRatationMatrix()*(point - cube->position);
	if ( abs(particle_CubeSpace.x) > cube->size || abs(particle_CubeSpace.y) > cube->size || abs(particle_CubeSpace.z) > cube->size )
		return false;
	else
		return true;
}
bool inline checkCollision_SegmentPlaneAlignY(vec4 start, vec4 end, float planePosition, float planeSize) {
	if ( start.y > planePosition && end.y > planePosition )return false;
	else if ( start.y < planePosition && end.y < planePosition )return false;
	vec4 dir = end - start;
	float crossMul = (planePosition - start.y) / dir.y;
	vec4 colPos = start + dir*crossMul;
	if ( abs(colPos.x) <= planeSize && abs(colPos.y) <= planeSize )
		return true;
	else
		return false;
}
bool inline checkCollision_SegmentPlaneAlignX(vec4 start, vec4 end, float planePosition, float planeSize) {
	return checkCollision_SegmentPlaneAlignY(eulerAngleZ(PI / 2)*start, eulerAngleZ(PI / 2)*end, planePosition, planeSize);
}
bool inline checkCollision_SegmentPlaneAlignZ(vec4 start, vec4 end, float planePosition, float planeSize) {
	return checkCollision_SegmentPlaneAlignY(eulerAngleX(-PI / 2)*start, eulerAngleX(-PI / 2)*end, planePosition, planeSize);
}
bool inline checkCollision_SegmentPlaneAlignZ() {

}
bool inline checkCollision_SegmentCube(vec4 start, vec4 end, Cube* cube) {
	vec4 startPoint = cube->getInverseRatationMatrix()*start;
	vec4 endPoint = cube->getInverseRatationMatrix()*end;
	float cubeSize = cube->size;
	vec4 line = endPoint - startPoint;
	//x-axis cube face
	float cross_multiply = (cubeSize - startPoint.x) / line.x;
	if ( startPoint.x > cubeSize && endPoint.x > cubeSize )return false;
	else if ( startPoint.x > cubeSize && endPoint.x <= cubeSize )return false;
	return false;
}
//completed
void inline checkCollision_SphereCube(Sphere* sph1, Cube* cube2) {
	if ( outOfBound_check(sph1, cube2) ) return;
	vec4 sph_CubeSpace = cube2->getInverseRatationMatrix()*(sph1->position - cube2->position);
	float totalSize = cube2->size + sph1->size;
	if ( abs(sph_CubeSpace.x) > totalSize || abs(sph_CubeSpace.y) > totalSize || abs(sph_CubeSpace.z) > totalSize ) return;
	for ( unsigned int i = 0; i < 12; i++ ) {
		vec4 start = (cube2->edgeSta[i]);
		vec4 end = (cube2->edgeEnd[i]);
		vec4 colPoint = min_dist_segment_to_point(start, end, sph1->position);
		if ( length(colPoint) <= sph1->radius ) {
			colSphere_Cube(sph1, cube2, colPoint);
			return;
		}
	}
	vec4 colPoint_CubeSpace = vec4(0);
	if ( 0 < sph_CubeSpace.x && sph_CubeSpace.x < totalSize && abs(sph_CubeSpace.y) < cube2->size && abs(sph_CubeSpace.z) < cube2->size )
		colPoint_CubeSpace = sph_CubeSpace - vec4(sph1->size, 0, 0, 0);
	else if ( 0 < sph_CubeSpace.y && sph_CubeSpace.y < totalSize  && abs(sph_CubeSpace.x) < cube2->size && abs(sph_CubeSpace.z) < cube2->size )
		colPoint_CubeSpace = sph_CubeSpace - vec4(0, sph1->size, 0, 0);
	else if ( 0 < sph_CubeSpace.z && sph_CubeSpace.z < totalSize  && abs(sph_CubeSpace.y) < cube2->size && abs(sph_CubeSpace.x)< cube2->size )
		colPoint_CubeSpace = sph_CubeSpace - vec4(0, 0, sph1->size, 0);
	else if ( 0 > sph_CubeSpace.x && sph_CubeSpace.x > -totalSize  && abs(sph_CubeSpace.y) < cube2->size && abs(sph_CubeSpace.z)< cube2->size )
		colPoint_CubeSpace = sph_CubeSpace + vec4(sph1->size, 0, 0, 0);
	else if ( 0 > sph_CubeSpace.y && sph_CubeSpace.y > -totalSize  && abs(sph_CubeSpace.x) < cube2->size && abs(sph_CubeSpace.z)< cube2->size )
		colPoint_CubeSpace = sph_CubeSpace + vec4(0, sph1->size, 0, 0);
	else if ( 0 > sph_CubeSpace.z && sph_CubeSpace.z > -totalSize  && abs(sph_CubeSpace.y) < cube2->size && abs(sph_CubeSpace.x) < cube2->size )
		colPoint_CubeSpace = sph_CubeSpace + vec4(0, 0, sph1->size, 0);
	else
		return;
	vec4 vec = sph_CubeSpace;
	vec4 colPoint_SphereSpace = cube2->getRotationMatrix()*colPoint_CubeSpace + (cube2->position - sph1->position);
	colSphere_Cube(sph1, cube2, colPoint_SphereSpace);
}
void inline checkCollision_SphereCylinder(Sphere* sph1, Cylinder* cylinder2) {
	if ( outOfBound_check(sph1, cylinder2) ) return;
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
//completed with infinite plane
void inline checkCollision_SpherePlane(Sphere* sph1, Plane* plane2) {
	if ( outOfBound_check(sph1, plane2) ) return;
	vec4 spPos = sph1->position;
	float radius = sph1->radius;
	vec4 centerVec = spPos - plane2->position;
	vec4 height = projectVec(centerVec, plane2->getNormal());
	if ( length(height) <= radius ) {
		colSphere_Plane(sph1, plane2, height);
	}
}
//completed
void inline checkCollision_SphereSphere(Sphere* sph1, Sphere* sph2) {
	if ( outOfBound_check(sph1, sph2) ) return;

	else colSphere_Sphere(sph1, sph2);
	//
}
//completed
void inline checkCollision_PlaneCube(Plane* plane1, Cube* cube2) {
	if ( outOfBound_check(plane1, cube2) ) return;
	vector<vec4> colPoint;
	vec4* vertex = cube2->transformVertex;
	for ( unsigned int i = 0; i < 8; i++ ) {
		if ( dot(vertex[i] - plane1->position, plane1->getNormal()) <= 0 ) {
			colPoint.push_back(vertex[i]);
		}
		if ( colPoint.size() >= 4 ) break;
	}
	if ( colPoint.size() != 0 )
		colCube_Plane(cube2, plane1, colPoint);
}
void inline checkCollision_PlaneCylinder(Plane* plane1, Cylinder* cylinder2) {
	if ( outOfBound_check(plane1, cylinder2) ) return;

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
//completed only the case that there are corner in cube
void inline checkCollision_CubeCube(Cube* cube1, Cube* cube2) {
	if ( outOfBound_check(cube1, cube2) ) return;
	vector<vec4> colPoint_Cube1Space;
	vector<vec4> colPoint_Cube2Space;
	vec4* vertex = cube2->transformVertex;
	for ( unsigned int i = 0; i < 8; i++ ) {
		if ( checkCollision_ParticleCube(vertex[i], cube1) )
			colPoint_Cube1Space.push_back(vertex[i]);
	}
	vertex = cube1->transformVertex;
	for ( unsigned int i = 0; i < 8; i++ ) {
		if ( checkCollision_ParticleCube(vertex[i], cube2) )
			colPoint_Cube1Space.push_back(vertex[i]);
	}
	if ( colPoint_Cube1Space.size() != 0 || colPoint_Cube2Space.size() != 0 ) {
		colCube_Cube(cube1, cube2, colPoint_Cube1Space, colPoint_Cube2Space);
		return;
	}

	//if ( projectSize(sph2->velocity - sph1->velocity, sph2->position - sph1->position) >= 0 ) return;
	//vec4 cubePos = cube1->position;
	//vec4 d = cubePos - cube2->position;
	//float distance = length(d);
	////float sumR = (size + cube2->size*1.4f) / 2;
	//if ( distance <= (cube1->boundedRadius + cube2->boundedRadius) / 2.0f ) {
	//	//onCollision
	//	colCube_Cube(cube1, cube2, (cube2->position - cube1->position) / 2.0f);
	//}
}
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
}

void inline checkCollision(vector<Cube*> cu, vector<Cylinder*> cy, vector<Plane*> pl, vector<Sphere*> sp) {
	//cout <<"size "<<cu.size()<<" "<<cy.size()<<" "<<pl.size()<<" "<<sp.size()<<"\n";
	for ( unsigned int i = 0; i < sp.size(); i++ ) {
		Sphere* sp1 = sp.at(i);
		for ( unsigned int j = 0; j < cu.size(); j++ ) checkCollision_SphereCube(sp1, cu.at(j));
		for ( unsigned int j = 0; j < cy.size(); j++ ) checkCollision_SphereCylinder(sp1, cy.at(j));
		for ( unsigned int j = 0; j < pl.size(); j++ ) checkCollision_SpherePlane(sp1, pl.at(j));
		if ( i < sp.size() - 1 ) {
			for ( unsigned int j = i + 1; j < sp.size(); j++ ) checkCollision_SphereSphere(sp1, sp.at(j));
		}
	}
	for ( unsigned int i = 0; i < pl.size(); i++ ) {
		Plane* pl1 = pl.at(i);
		for ( unsigned int j = 0; j < cu.size(); j++ ) checkCollision_PlaneCube(pl1, cu.at(j));
		for ( unsigned int j = 0; j < cy.size(); j++ ) checkCollision_PlaneCylinder(pl1, cy.at(j));
	}
	for ( unsigned int i = 0; i < cu.size(); i++ ) {
		Cube* cu1 = cu.at(i);
		if ( i < cu.size() - 1 ) for ( unsigned int j = i + 1; j < cu.size(); j++ ) checkCollision_CubeCube(cu1, cu.at(j));
		for ( unsigned int j = 0; j < cy.size(); j++ ) checkCollision_CubeCylinder(cu1, cy.at(j));
	}
	for ( unsigned int i = 0; i < cy.size(); i++ ) {
		Cylinder* cy1 = cy.at(i);
		if ( i < cy.size() - 1 )
		for ( unsigned int j = i + 1; j < cy.size(); j++ )
			checkCollision_CylinderCylinder(cy1, cy.at(j));

	}
}
#endif // !COLLISION_DETECTION_CPP