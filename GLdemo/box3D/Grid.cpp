#include "Grid.h"

Grid::Grid() {}
Grid::Grid(vector<Plane*>pl) {

	width = 1;
	for ( unsigned int i = 0; i < gridSize; i++ ) {
		for ( unsigned int j = 0; j < gridSize; j++ ) {
			for ( unsigned int k = 0; k < gridSize; k++ ) {
				//gridcell[i][j][k] = GridCell();
			}
		}
	}
	//gridcell[i][j][k] = GridCell(begin_x+width*i,begin_y+width*j,begin_z+width*k);
	//hashGrid(cu,cy,sp);
	for ( unsigned int i = 0; i < pl.size(); i++ ) {
		hashPlane((pl[i]));
	}
}
Grid::~Grid() {}
//GridCell gridcell[gridSize][gridSize][gridSize];
GridCell gridcell;
void Grid::hashGrid(vector<Cube*> cu, vector<Cylinder*> cy, vector<Sphere*> sp) {
	for ( unsigned int i = 0; i < cu.size(); i++ ) hashCube(cu[i]);
	for ( unsigned int i = 0; i < cy.size(); i++ ) hashCylinder(cy[i]);
	for ( unsigned int i = 0; i < sp.size(); i++ ) hashSphere(sp[i]);
}
void Grid::hashCube(Cube* r) {
	gridcell.addCubeToGridCell(r);
	//vec4 pos = r->position;
	////findIndex(vec3(pos.x,pos.y,pos.z),a,b,c);
	//vector<int> x;
	//vector<int> y;
	//vector<int> z;
	//findGrid(vec3(pos.x, pos.y, pos.z), r->getSkin(), x, y, z);
	//for ( unsigned int i = 0; i < x.size(); i++ ) {
	//	if ( x[i] >= 0 && x[i] < gridSize && y[i] >= 0 && y[i] < gridSize && z[i] >= 0 && z[i] < gridSize )
	//		gridcell[x[i]][y[i]][z[i]].addCubeToGridCell(r);
	//}
}
void Grid::hashCylinder(Cylinder* r) {
	gridcell.addCylinderToGridCell(r);
	//vec4 pos = r->position;
	////findIndex(vec3(pos.x,pos.y,pos.z),a,b,c);
	//vector<int> x;
	//vector<int> y;
	//vector<int> z;
	//findGrid(vec3(pos.x, pos.y, pos.z), r->getSkin(), x, y, z);
	//for ( unsigned int i = 0; i < x.size(); i++ ) {
	//	if ( x[i] >= 0 && x[i] < gridSize && y[i] >= 0 && y[i] < gridSize && z[i] >= 0 && z[i] < gridSize )
	//		gridcell[x[i]][y[i]][z[i]].addCylinderToGridCell(r);
	//}
}
void Grid::hashSphere(Sphere* r) {
	gridcell.addSphereToGridCell(r);
	//vec4 pos = r->position;
	////findIndex(vec3(pos.x,pos.y,pos.z),a,b,c);
	//vector<int> x;
	//vector<int> y;
	//vector<int> z;
	//findGrid(vec3(pos.x, pos.y, pos.z), r->getSkin(), x, y, z);
	//for ( unsigned int i = 0; i < x.size(); i++ ) {
	//	if ( x[i] >= 0 && x[i] < gridSize && y[i] >= 0 && y[i] < gridSize && z[i] >= 0 && z[i] < gridSize )
	//		gridcell[x[i]][y[i]][z[i]].addSphereToGridCell(r);
	//}
}
void Grid::hashPlane(Plane* r) {
	gridcell.addPlaneToGridCell(r);
	//vec4 pos = r->position;
	//int a, b, c = 0;
	//findIndex(vec3(pos.x, pos.y, pos.z), a, b, c);
	//for ( unsigned int i = 0; i < gridSize; i++ ) {
	//	for ( unsigned int j = 0; j < gridSize; j++ ) {
	//		for ( unsigned int k = 0; k < gridSize; k++ ) {
	//			if ( r->orientation == vec3(0, 0, PI / 2) || r->orientation == vec3(0, 0, -PI / 2) ) {
	//				gridcell[a][j][k].addPlaneToGridCell(r);
	//			}
	//			if ( r->orientation == vec3(PI / 2, 0, 0) || r->orientation == vec3(-PI / 2, 0, 0) ) {
	//				gridcell[i][j][c].addPlaneToGridCell(r);
	//			}
	//			if ( r->orientation == vec3(0, 0, 0) || r->orientation == vec3(PI, 0, 0) ) {
	//				//cout<<"plane x = "<<r->orientation.x<<" y = "<<r->orientation.y<<" z = "<<r->orientation.z<<"\n";
	//				//cout<<"index x= "<< a<<" y= "<<b<<" z= "<<c<<"\n";
	//				gridcell[i][b][k].addPlaneToGridCell(r);
	//			}
	//		}
	//	}
	//}
}
void Grid::findIndex(vec3 pos, int &i, int &j, int &k) {
	i = (int)(pos.x - begin_x) / width;
	j = (int)(pos.y - begin_y) / width;
	k = (int)(pos.z - begin_z) / width;
}
void Grid::clearGridPlane() {
	gridcell.clearGridCellPlane();
	//for ( unsigned int i = 0; i < gridSize; i++ )
	//for ( unsigned int j = 0; j < gridSize; j++ )
	//for ( unsigned int k = 0; k < gridSize; k++ )
	//	gridcell[i][j][k].clearGridCellPlane();
}
void Grid::clearGrid() {
	gridcell.clearGridCell();
	//for ( unsigned int i = 0; i < gridSize; i++ )
	//for ( unsigned int j = 0; j < gridSize; j++ )
	//for ( unsigned int k = 0; k < gridSize; k++ )
	//	gridcell[i][j][k].clearGridCell();

}
void Grid::checkCollisionGrid() {
	gridcell.checkCollisionGridCell();
	//for ( unsigned int i = 0; i < gridSize; i++ )
	//for ( unsigned int j = 0; j < gridSize; j++ )
	//for ( unsigned int k = 0; k < gridSize; k++ ) {
	//	gridcell[i][j][k].checkCollisionGridCell();
	//}
}
void Grid::findGrid(vec3 pos, vec3 skin, vector<int> &x, vector<int> &y, vector<int> &z) {
	//x.push_back(a); y.push_back(b); z.push_back(c);
	//if(pos.x-floor(pos.x)==0.5 && pos.y-floor(pos.y)==0.5 && pos.z-floor(pos.z)==0.5) return;
	vec3 minPos = vec3(pos.x - skin.x / 2.0, pos.y - skin.y / 2.0, pos.z - skin.z / 2.0);
	vec3 maxPos = vec3(pos.x + skin.x / 2.0, pos.y + skin.y / 2.0, pos.z + skin.z / 2.0);
	int minA, minB, minC;
	int maxA, maxB, maxC;
	findIndex(minPos, minA, minB, minC);
	findIndex(maxPos, maxA, maxB, maxC);
	for ( int i = minA; i <= maxA; i++ ) {
		for ( int j = minB; j <= maxB; j++ ) {
			for ( int k = minC; k <= maxC; k++ ) {
				x.push_back(i);
				y.push_back(j);
				z.push_back(k);
			}
		}
	}
	//if(pos.x-floor(pos.x) < 0.5) {										//x<0.5 
	//	x.push_back(a-1); y.push_back(b); z.push_back(c);
	//	if(pos.y-floor(pos.y) < 0.5) {									//y<0.5
	//		x.push_back(a-1); y.push_back(b-1); z.push_back(c);
	//		if(pos.z-floor(pos.z) < 0.5) {								//z<0.5
	//			x.push_back(a-1); y.push_back(b-1); z.push_back(c-1); 
	//		} else {													//z>=0.5
	//			x.push_back(a-1); y.push_back(b-1); z.push_back(c+1); 
	//		}
	//	} else {														//y>=0.5
	//		x.push_back(a-1); y.push_back(b+1); z.push_back(c); 
	//		if(pos.z-floor(pos.z) < 0.5) {								//z<0.5
	//			x.push_back(a-1); y.push_back(b+1); z.push_back(c-1); 
	//		} else {													//z>=0.5
	//			x.push_back(a-1); y.push_back(b+1); z.push_back(c+1); 
	//		}
	//	}
	//} else {															//x>=0.5 
	//	x.push_back(a+1); y.push_back(b); z.push_back(c); 
	//	if(pos.y-floor(pos.y) < 0.5) {									//y<0.5 
	//		x.push_back(a+1); y.push_back(b-1); z.push_back(c); 
	//		if(pos.z-floor(pos.z) < 0.5) {								//z<0.5
	//			x.push_back(a+1); y.push_back(b-1); z.push_back(c-1); 
	//		} else {													//z>=0.5 
	//			x.push_back(a+1); y.push_back(b-1); z.push_back(c+1); 
	//		}
	//	} else {														//y>=0.5
	//		x.push_back(a+1); y.push_back(b+1); z.push_back(c); 
	//		if(pos.z-floor(pos.z) < 0.5) {								//z<0.5
	//			x.push_back(a+1); y.push_back(b+1); z.push_back(c-1); 
	//		} else {													//z>=0.5 
	//			x.push_back(a+1); y.push_back(b+1); z.push_back(c+1); 
	//		}
	//	}
	//}
	//
	//if(pos.y-floor(pos.y) < 0.5) {										//y<0.5
	//	x.push_back(a); y.push_back(b-1); z.push_back(c); 
	//	if(pos.z-floor(pos.z) < 0.5) {									//z<0.5
	//		x.push_back(a); y.push_back(b-1); z.push_back(c-1); 
	//	} else {														//z>=0.5
	//		x.push_back(a); y.push_back(b-1); z.push_back(c+1); 
	//	}
	//} else { 
	//	x.push_back(a); y.push_back(b+1); z.push_back(c); 
	//	if(pos.z-floor(pos.z) < 0.5) { 
	//		x.push_back(a); y.push_back(b+1); z.push_back(c-1); 
	//	} else { 
	//		x.push_back(a); y.push_back(b+1); z.push_back(c+1); 
	//	}
	//}
	//
	//if(pos.z-floor(pos.z) < 0.5) { 
	//	x.push_back(a); y.push_back(b); z.push_back(c-1); 
	//	if(pos.x-floor(pos.x) < 0.5) {
	//		x.push_back(a-1); y.push_back(b); z.push_back(c-1);
	//	} else {
	//		x.push_back(a+1); y.push_back(b); z.push_back(c-1);
	//	}
	//} else { 
	//	x.push_back(a); y.push_back(b); z.push_back(c+1); 
	//	if(pos.x-floor(pos.x) < 0.5) {
	//		x.push_back(a-1); y.push_back(b); z.push_back(c+1);
	//	} else {
	//		x.push_back(a+1); y.push_back(b); z.push_back(c+1);
	//	}
	//}

}