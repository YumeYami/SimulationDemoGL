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
#include "controls.hpp"
#include "demo.h"
#include "box3D/box3Dcollision.cpp"

#define GRAVITY 0.05f
#define GLOBAL_FRICTION 0.01f

using namespace glm;
using namespace std;

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

vector<Cube*> c3;
vector<Sphere*> sphere;
vector<Cylinder*> cylinder;
vector<Plane*> plane;
Grid grid;

void addSphere() {
	vec3 position = vec3(rand() % (gridSize - 5) - 2, begin_x + gridSize - 4, rand() % (gridSize - 5) - 2);
	vec3 rotation = vec3(0, 0, 1);
	vec3 velocity = vec3(rand() % 2 / 2.0, -rand() % 2 / 2.0, rand() % 2 / 2.0);
	float size = rand() % 25 / 10.0 + 0.5;
	float mass = 1;
	vec3 color = vec3(rand() % 11 / 10.0, rand() % 11 / 10.0, rand() % 11 / 10.0);
	Sphere* sp = new Sphere(position, rotation, velocity, size, mass, color);
	sphere.push_back(sp);
}
void addCube() {
	vec3 position = vec3(rand() % (gridSize - 5) - 2, begin_x + gridSize - 4, rand() % (gridSize - 5) - 2);
	vec3 rotation = vec3(0, 0, 1);
	vec3 velocity = vec3(rand() % 4 / 15.0, -rand() % 4 / 15.0, rand() % 4 / 15.0);
	float size = rand() % 20 / 10 + 0.5;
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
	vec3 pos[6] = {vec3(1.5, begin_y + gridSize - 1.5, 1.5), vec3(1.5, begin_y + 1.5, 1.5), vec3(begin_x + 1.5, 1.5, 1.5), vec3(begin_x + gridSize - 1.5, 1.5, 1.5), vec3(1.5, 1.5, begin_z + gridSize - 1.5), vec3(1.5, 1.5, begin_z + 1.5)};
	vec3 rot[6] = {vec3(PI, 0, 0), vec3(0, 0, 0), vec3(0, 0, -PI / 2), vec3(0, 0, PI / 2), vec3(-PI / 2, 0, 0), vec3(PI / 2, 0, 0)};
	for ( int i = 0; i < 6; i++ ) {
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
	for ( int i = 0; i < 6; i++ ) {
		if ( plane[i]->color.a == 0.2f ) plane[i]->color.a = 0.8f;
		else plane[i]->color.a = 0.2f;
	}
}
int lastkey[10];

//initial openGL
//testing document
int initOpenGL() {
	// Initialise GLFW
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
		if ( update || playOneFrame ) {
			grid.checkCollisionGrid();
		}
		computeMatricesFromInputs();// Compute the MVP matrix from keyboard and mouse input
		grid.clearGrid();
		onPress();
		glm::mat4 ModelMatrix = mat4(1.0f);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		GLuint matrixIDs[] = {ScaleMatrixID, RotateMatrixID, TranslateMatrixID, TranslateModelID, RotateModelID};
		for ( int i = 0; i < c3.size(); i++ ) {
			Rigidbody* cubeObject = dynamic_cast<Rigidbody*>(c3[i]);
			cubeObject->renderObject(matrixIDs, timeStep, GRAVITY*enGravity, update, playOneFrame);
		}
		for ( int i = 0; i < sphere.size(); i++ ) {
			Rigidbody* sphereObject = dynamic_cast<Rigidbody*>(sphere[i]);
			sphereObject->renderObject(matrixIDs, timeStep, GRAVITY*enGravity, update, playOneFrame);
		}
		for ( int i = 0; i < cylinder.size(); i++ ) {
			Rigidbody* cylinderObject = dynamic_cast<Rigidbody*>(cylinder[i]);
			cylinderObject->renderObject(matrixIDs, timeStep, GRAVITY*enGravity, update, playOneFrame);
		}
		for ( int i = 0; i < plane.size(); i++ ) {
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