#ifndef GRIDCELL_H
#define GRIDCELL_H

#include "CollisionDetection.cpp"

class GridCell {
public:
	vector<Cube*> cube;
	vector<Cylinder*> cylinder;
	vector<Sphere*> sphere;
	vector<Plane*> plane;
	GridCell();
	~GridCell();
	void addCubeToGridCell(Cube* cu);
	void addCylinderToGridCell(Cylinder* cy);
	void addSphereToGridCell(Sphere* sp);
	void addPlaneToGridCell(Plane* pl);
	void clearGridCellPlane();
	void clearGridCell();
	void checkCollisionGridCell();
};
#endif // !GRIDCELL_H
