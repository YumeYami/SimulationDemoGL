// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
// Realtime - Project library
#include <GL/glew.h>
#include <GL/glfw.h>
#include <GL/GLU.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <common/shader.cpp>

#include "box3D/CollisionDetection.cpp"
#include "controls.hpp"

using namespace glm;
using namespace std;

#define GRAVITY 0.05f
#define GLOBAL_FRICTION 0.01f

vector<Cube*> c3;
vector<Sphere*> sphere;
vector<Cylinder*> cylinder;
vector<Plane*> plane;

Grid grid;
void addSphereTest() {
	vec3 position = vec3(0, begin_x + gridSize - 2, 0);
	vec3 velocity = vec3(0);
	vec3 rotation = vec3(0, 0, 0);
	float size = 2;
	float mass = 1;
	vec3 color = vec3(rand() % 11 / 10.0, rand() % 11 / 10.0, rand() % 11 / 10.0);
	Sphere* sp = new Sphere(position, rotation, velocity, size, mass, color);
	sphere.push_back(sp);
}
void addSphereTest2() {
	vec3 position = vec3(0, begin_x + gridSize - 4, 0);
	vec3 velocity = vec3(0);
	vec3 rotation = vec3(0, 0, 0);
	float size = 2;
	float mass = 1;
	vec3 color = vec3(rand() % 11 / 10.0, rand() % 11 / 10.0, rand() % 11 / 10.0);
	Sphere* sp = new Sphere(position, rotation, velocity, size, mass, color);
	sphere.push_back(sp);
}
void addCubeTest() {
	vec3 position = vec3(0, begin_x + gridSize - 4, 0);
	vec3 rotation = vec3(0, 0, 0);
	//vec3 velocity = vec3(rand() % 4 / 15.0, -rand() % 4 / 15.0, rand() % 4 / 15.0);
	vec3 velocity = vec3(0);
	float size = 2;
	float mass = 1;
	vec3 color = vec3(rand() % 11 / 10.0, rand() % 11 / 10.0, rand() % 11 / 10.0);
	Cube *cube = new Cube(position, rotation, velocity, size, mass, color);
	//cube->angularVelocity = vec3(rand() % 4 / 10.0, -rand() % 4 / 10.0, rand() % 4 / 10.0);
	c3.push_back(cube);
}
void addCylinderTest() {
	vec3 position = vec3(0, 4, 0);
	vec3 rotation = vec3(0);
	// vec3(rand() % 20 / 2, rand() % 20 / 2, rand() % 20 / 2);
	vec3 velocity = vec3(rand() % 4 / 10.0, -rand() % 4 / 10.0, rand() % 4 / 10.0);
	float radius = 1;
	float length = 2;
	float mass = 1;
	vec3 color = vec3(rand() % 11 / 10.0, rand() % 11 / 10.0, rand() % 11 / 10.0);
	Cylinder *cy = new Cylinder(position, rotation, velocity, radius, length, mass, color);
	cylinder.push_back(cy);
}
void addSphere() {
	vec3 position = vec3(rand() % (gridSize - 5) - 2, begin_x + gridSize - 4, rand() % (gridSize - 5) - 2);
	//vec3 position = vec3((gridSize - 5), begin_x + gridSize - 4, (gridSize - 5));
	vec3 rotation = vec3(0, 0, 1);
	vec3 velocity = vec3(rand() % 2 / 2.0, -rand() % 2 / 2.0, rand() % 2 / 2.0);
	float size = rand() % 25 / 10.0f + 0.5f;
	float mass = 1;
	vec3 color = vec3(rand() % 11 / 10.0, rand() % 11 / 10.0, rand() % 11 / 10.0);
	Sphere* sp = new Sphere(position, rotation, velocity, size, mass, color);
	sphere.push_back(sp);
}
void addCube() {
	vec3 position = vec3(rand() % (gridSize - 5) - 2, begin_x + gridSize - 4, rand() % (gridSize - 5) - 2);
	//vec3 position = vec3(0, begin_x + gridSize - 4, 0 );
	vec3 rotation = vec3(0, 0, 1);
	vec3 velocity = vec3(rand() % 4 / 15.0, -rand() % 4 / 15.0, rand() % 4 / 15.0);
	float size = rand() % 20 / 10 + 0.5f;
	float mass = 1;
	vec3 color = vec3(rand() % 11 / 10.0, rand() % 11 / 10.0, rand() % 11 / 10.0);
	Cube *cube = new Cube(position, rotation, velocity, size, mass, color);
	cube->angularVelocity = vec3(rand() % 4 / 10.0, -rand() % 4 / 10.0, rand() % 4 / 10.0);
	c3.push_back(cube);
}
void addCylinder() {
	vec3 position = vec3(rand() % (gridSize - 5) - 2, begin_x + gridSize - 4, rand() % (gridSize - 5) - 2);
	vec3 rotation = vec3(rand() % (gridSize - 5) - 2, begin_x + gridSize - 4, rand() % (gridSize - 5) - 2);
	vec3 velocity = vec3(rand() % 4 / 10.0, -rand() % 4 / 10.0, rand() % 4 / 10.0);
	float radius = 0.25;
	float length = 0.5f;
	float mass = 1;
	vec3 color = vec3(rand() % 11 / 10.0, rand() % 11 / 10.0, rand() % 11 / 10.0);
	Cylinder *cy = new Cylinder(position, rotation, velocity, radius, length, mass, color);
	cylinder.push_back(cy);
}
void addPlane() {
	//top,bottom,left,right,front,back
	vec3 pos[6] = {vec3(1.5, begin_y + gridSize - 1.5, 1.5), vec3(1.5, begin_y + 1.5, 1.5), vec3(begin_x + 1.5, 1.5, 1.5),
		vec3(begin_x + gridSize - 1.5, 1.5, 1.5), vec3(1.5, 1.5, begin_z + gridSize - 1.5), vec3(1.5, 1.5, begin_z + 1.5)};
	vec3 rot[6] = {vec3(PI, 0, 0), vec3(0, 0, 0), vec3(0, 0, -PI / 2), vec3(0, 0, PI / 2), vec3(-PI / 2, 0, 0), vec3(PI / 2, 0, 0)};
	for ( unsigned int i = 0; i < 6; i++ ) {
		vec3 position = pos[i];
		vec3 rotation = rot[i];
		vec3 velocity = vec3(0, 0, 0);
		float size = 20;
		float mass = 1;
		vec3 color = vec3(rand() % 11 / 10.0, rand() % 11 / 10.0, rand() % 11 / 10.0);
		Plane *pl = new Plane(position, rotation, velocity, size, mass, color);
		plane.push_back(pl);
	}
}
void transparentPlane() {
	//top,bottom,left,right,front, back
	for ( unsigned int i = 0; i < 6; i++ ) {
		if ( plane[i]->transparent == 0.2f ) plane[i]->transparent = 0.8f;
		else plane[i]->transparent = 0.2f;
	}
}

int lastkey[10];
int lastKey1 = GLFW_RELEASE;
int lastKey2 = GLFW_RELEASE;
int lastKey3 = GLFW_RELEASE;
int lastKey4 = GLFW_RELEASE;
int lastKey5 = GLFW_RELEASE;
int lastKey6 = GLFW_RELEASE;
int lastKey7 = GLFW_RELEASE;
int lastKey8 = GLFW_RELEASE;
int lastKey9 = GLFW_RELEASE;
int lastKey0 = GLFW_RELEASE;
int lastMouse = GLFW_RELEASE;
int lastKeyZ = GLFW_RELEASE;
int lastKeyX = GLFW_RELEASE;
int lastKeyC = GLFW_RELEASE;
int fixX = 0, fixY = 0;
int showX = 0, showY = 0;
int clickX1, clickY1 = 0;
int clickX2, clickY2 = 0;
int xposL, yposL;
int pickObject = 0;
int width = 512;
int height = 384;
int update = 1;
int playFrame = 1;
int playOneFrame = 0;
int enGravity = 0;

void pick(int mouse_x, int mouse_y) {
	float x = (2.0f * mouse_x) / width - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / height;
	float z = 1.0f;
	vec3 ray_nds = vec3(x, y, z);
	vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	vec4 ray_eye = inverse(getProjectionMatrix()) * ray_clip;
	ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	vec3 ray_wor = (vec3)(inverse(getViewMatrix()) * ray_eye);
	// don't forget to normalize the vector at some point
	ray_wor = normalize(ray_wor);
	//cout<<"ray x= "<<ray_wor.x<<" y = "<<ray_wor.y<<" z = "<<ray_wor.z<<"\n";
}
//keyboard function
void onPress() {
	//sphere
	if ( glfwGetKey('1') == GLFW_PRESS ) {
		if ( lastKey1 == GLFW_RELEASE ) addSphere();
		lastKey1 = GLFW_PRESS;

	}
	else if ( glfwGetKey('1') == GLFW_RELEASE ) {
		lastKey1 = GLFW_RELEASE;
	}

	//cube
	if ( glfwGetKey('2') == GLFW_PRESS ) {
		if ( lastKey2 == GLFW_RELEASE ) addCube();
		lastKey2 = GLFW_PRESS;
	}
	else if ( glfwGetKey('2') == GLFW_RELEASE ) {
		lastKey2 = GLFW_RELEASE;
	}

	//cylinder
	if ( glfwGetKey('3') == GLFW_PRESS ) {
		if ( lastKey3 == GLFW_RELEASE ) addCylinder();
		lastKey3 = GLFW_PRESS;
	}
	else if ( glfwGetKey('3') == GLFW_RELEASE ) {
		lastKey3 = GLFW_RELEASE;
	}

	//plane
	if ( glfwGetKey('4') == GLFW_PRESS ) {
		if ( lastKey4 == GLFW_RELEASE ) transparentPlane();
		lastKey4 = GLFW_PRESS;
	}
	else if ( glfwGetKey('4') == GLFW_RELEASE ) {
		lastKey4 = GLFW_RELEASE;
	}
	//////////////////////////////////////////////////////////////////////////
	//test
	if ( glfwGetKey('5') == GLFW_PRESS ) {
		if ( lastKey5 == GLFW_RELEASE ) addSphereTest();
		lastKey5 = GLFW_PRESS;
	}
	else if ( glfwGetKey('5') == GLFW_RELEASE ) {
		lastKey5 = GLFW_RELEASE;
	}
	if ( glfwGetKey('6') == GLFW_PRESS ) {
		if ( lastKey6 == GLFW_RELEASE ) addCubeTest();
		lastKey6 = GLFW_PRESS;

	}
	else if ( glfwGetKey('6') == GLFW_RELEASE ) {
		lastKey6 = GLFW_RELEASE;
	}
	if ( glfwGetKey('7') == GLFW_PRESS ) {
		if ( lastKey7 == GLFW_RELEASE ) addCylinderTest();
		lastKey7 = GLFW_PRESS;

	}
	else if ( glfwGetKey('7') == GLFW_RELEASE ) {
		lastKey7 = GLFW_RELEASE;
	}


	if ( glfwGetKey('0') == GLFW_PRESS ) {
		if ( lastKey0 == GLFW_RELEASE ) {
			if ( pickObject ) pickObject = 0;
			else pickObject = 1;
		}
		lastKey0 = GLFW_PRESS;
	}
	else if ( glfwGetKey('0') == GLFW_RELEASE ) {
		lastKey0 = GLFW_RELEASE;
	}
	if ( glfwGetKey('Z') == GLFW_PRESS ) {
		if ( lastKeyZ == GLFW_RELEASE )
		if ( update ) {
			update = 0;

		}
		else {
			update = 1;
			playFrame = 1;
		}
		lastKeyZ = GLFW_PRESS;

	}
	else if ( glfwGetKey('Z') == GLFW_RELEASE ) {
		lastKeyZ = GLFW_RELEASE;

	}
	if ( glfwGetKey('X') == GLFW_PRESS && !update ) {
		if ( lastKeyX == GLFW_RELEASE ) {
			playFrame = 0;
			playOneFrame = 1;
		}
		lastKeyX = GLFW_PRESS;
	}
	else if ( glfwGetKey('X') == GLFW_RELEASE ) {
		lastKeyX = GLFW_RELEASE;
	}
	if ( glfwGetKey('C') == GLFW_PRESS ) {
		if ( lastKeyC == GLFW_RELEASE ) {
			sphere.clear();
			c3.clear();
			cylinder.clear();
		}
		lastKeyC = GLFW_PRESS;
	}
	else if ( glfwGetKey('C') == GLFW_RELEASE ) {
		lastKeyC = GLFW_RELEASE;
	}
	if ( glfwGetKey('G') == GLFW_PRESS ) {
		if ( lastKey9 == GLFW_RELEASE ) {
			if ( enGravity ) enGravity = 0;
			else enGravity = 1;
		}
		lastKey9 = GLFW_PRESS;
	}
	else if ( glfwGetKey('G') == GLFW_RELEASE ) {
		lastKey9 = GLFW_RELEASE;
	}
	//pickBox=========================================================================================
	if ( lastMouse == GLFW_RELEASE && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !pickObject ) {
		glfwGetMousePos(&clickX1, &clickY1);
		pick(clickX1, clickY1);
		lastMouse = GLFW_PRESS;
	}
	else if ( lastMouse == GLFW_PRESS && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !pickObject ) {
		glfwGetMousePos(&clickX2, &clickY2);
		float dx = clickX2 - clickX1;
		float dy = clickY2 - clickY1;
		clickX1 = clickX2;
		clickY1 = clickY2;
		dx /= 200.0f;
		dy /= -200.0f;
		float topPos = plane[0]->position.y;
		float btmPos = plane[1]->position.y;
		float lftPos = plane[2]->position.x;
		float rhtPos = plane[3]->position.x;
		vec4 pos = vec4(0, 0, 0, 0);
		if ( topPos + dy <= begin_y + gridSize && btmPos + dy > begin_y ) pos.y = dy;
		if ( lftPos + dx >= begin_x && rhtPos + dx < begin_x + gridSize ) pos.x = dx;
		grid.clearGridPlane();
		for ( unsigned int i = 0; i < plane.size(); i++ ) {
			plane[i]->position += pos;
			grid.hashPlane(plane[i]);
		}
	}
	else if ( glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && !pickObject ) {
		lastMouse = GLFW_RELEASE;
	}
	//pickObject=========================================================================================
	if ( lastMouse == GLFW_RELEASE && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && pickObject ) {
		glfwGetMousePos(&clickX1, &clickY1);
		lastMouse = GLFW_PRESS;
	}
	else if ( lastMouse == GLFW_PRESS && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && pickObject ) {
	}
	else if ( glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && pickObject ) {
		lastMouse = GLFW_RELEASE;
	}
}
//initial openGL
int initOpenGL() {
	// Initialize GLFW
	if ( !glfwInit() ) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	//GLUTkeyboardfun(keyboard);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
	// Open a window and create its OpenGL context
	if ( !glfwOpenWindow(1024, 768, 0, 0, 0, 0, 32, 0, GLFW_WINDOW) ) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	// Initialize GLEW
	if ( glewInit() != GLEW_OK ) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	glfwSetWindowTitle("3D Rigid Body Physics Simulation");
	// Ensure we can capture the escape key being pressed below
	glfwEnable(GLFW_STICKY_KEYS);
	glfwSetMousePos(1024 / 2, 768 / 2);
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE); // Not this time !
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return 0;
}
//main function
int main(void) {
	if ( initOpenGL() == -1 )return -1;
	// Create and compile our GLSL program from the shader
	GLuint programID2 = LoadShaders("StandardShading.vertexshader", "StandardTransparentShading.fragmentshader");
	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID2, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID2, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID2, "M");
	GLuint ScaleMatrixID = glGetUniformLocation(programID2, "Scale");
	GLuint TranslateMatrixID = glGetUniformLocation(programID2, "Translate");
	GLuint RotateMatrixID = glGetUniformLocation(programID2, "Rotate");
	GLuint TranslateModelID = glGetUniformLocation(programID2, "TranslateModel");
	GLuint RotateModelID = glGetUniformLocation(programID2, "RotateModel");
	//---------------------------------------------------------------------------------------------------------------
	addPlane();
	grid = Grid(plane);

	float lastTime = glfwGetTime();
	int nbFrames = 0;

	do {
		// Measure speed
		float currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ) { // If last prinf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", 1000.0 / float(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear the screen
		glUseProgram(programID2);// Use our shader
		grid.hashGrid(c3, cylinder, sphere);
		onPress();
		if ( update || playOneFrame ) {
			grid.checkCollisionGrid();
		}
		computeMatricesFromInputs();// Compute the MVP matrix from keyboard and mouse input
		grid.clearGrid();

		glm::mat4 ModelMatrix = mat4(1.0f);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		GLuint matrixIDs[] = {ScaleMatrixID, RotateMatrixID, TranslateMatrixID, TranslateModelID, RotateModelID};
		for ( unsigned int i = 0; i < c3.size(); i++ ) {
			Rigidbody* cubeObject = dynamic_cast<Rigidbody*>(c3[i]);
			cubeObject->renderObject(matrixIDs, timeStep, GRAVITY*enGravity, update, playOneFrame);
			//cubeObject->updatePosition(1,1);
		}
		for ( unsigned int i = 0; i < sphere.size(); i++ ) {
			Rigidbody* sphereObject = dynamic_cast<Rigidbody*>(sphere[i]);
			sphereObject->renderObject(matrixIDs, timeStep, GRAVITY*enGravity, update, playOneFrame);
		}
		for ( unsigned int i = 0; i < cylinder.size(); i++ ) {
			Rigidbody* cylinderObject = dynamic_cast<Rigidbody*>(cylinder[i]);
			cylinderObject->renderObject(matrixIDs, timeStep, GRAVITY*enGravity, update, playOneFrame);
		}
		for ( unsigned int i = 0; i < plane.size(); i++ ) {
			Rigidbody* planeObject = dynamic_cast<Rigidbody*>(plane[i]);
			planeObject->renderObject(matrixIDs, 0.01f, 0, update, playOneFrame);
		}
		// Swap buffers
		glfwSwapBuffers();
		playOneFrame = 0;
	} // Check if the ESC key was pressed or the window was closed
	while ( glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
	glfwGetWindowParam(GLFW_OPENED) );
	glDeleteProgram(programID2);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

