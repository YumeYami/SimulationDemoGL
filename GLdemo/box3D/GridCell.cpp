#include "GridCell.h"
#include "CollisionDetection.cpp"
GridCell::GridCell() {}
GridCell::~GridCell() {}

void GridCell::addCubeToGridCell(Cube* cu) {
	cube.push_back(cu);
}
void GridCell::addCylinderToGridCell(Cylinder* cy) {
	cylinder.push_back(cy);
}
void GridCell::addSphereToGridCell(Sphere* sp) {
	sphere.push_back(sp);
}
void GridCell::addPlaneToGridCell(Plane* pl) {
	plane.push_back(pl);
}
void GridCell::clearGridCellPlane() {
	plane.clear();
}
void GridCell::clearGridCell() {
	cube.clear();
	cylinder.clear();
	sphere.clear();
}
void GridCell::checkCollisionGridCell() {
	checkCollision(cube, cylinder, plane, sphere);
}