#ifndef GRID_H
#define GRID_H

#define gridSize 13
#define begin_x -5
#define begin_y -5
#define begin_z -5
#include "CollisionDetection.cpp"
#include "GridCell.h"

class Grid {
public:
	
	float width;
	Grid();
	Grid(vector<Plane*>pl);
	~Grid();
	
	void hashGrid(vector<Cube*> cu, vector<Cylinder*> cy, vector<Sphere*> sp);
	void hashCube(Cube* r);
	void hashCylinder(Cylinder* r);
	void hashSphere(Sphere* r);
	void hashPlane(Plane* r);
	void findIndex(vec3 pos, int &i, int &j, int &k);
	void clearGridPlane();
	void clearGrid();
	void checkCollisionGrid();
	void findGrid(vec3 pos, vec3 skin, vector<int> &x, vector<int> &y, vector<int> &z);
};

#endif // !GRID_H
