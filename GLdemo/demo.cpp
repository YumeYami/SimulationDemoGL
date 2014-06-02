// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
// Realtime - Project library
#include "box3D/box3Dcollision.cpp"

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GL/glfw.h>
// Include GLU
#include <GL/GLU.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;
#include <common/shader.cpp>
//#include <common/texture.cpp>
//#include <common/controls.cpp>
#include "controls.hpp"
#define GRAVITY 0.05f
#define GLOBAL_FRICTION 0.01f

std::vector<unsigned short> indices;
std::vector<glm::vec3> indexed_vertices;
std::vector<glm::mat4> indexed_rotates;
std::vector<glm::vec2> indexed_uvs;
std::vector<glm::vec3> indexed_normals;

vector<Cube*> c3;
vector<Sphere*> sphere;
vector<Cylinder*> cylinder;
vector<Plane*> plane;

Grid grid;

void addSphere(){
	//0->8
	//-2->6
	/*vec3 position = vec3(rand()%(gridSize-5)-2,begin_x+gridSize-4,rand()%(gridSize-5)-2);*/
	//vec3 position = vec3((gridSize - 5) , begin_x + gridSize - 4, (gridSize - 5) );
	vec3 position = vec3((gridSize - 5) - 2, begin_x + gridSize - 4, (gridSize - 5) - 2);
	vec3 rotation = vec3(0, 0, 1);
	vec3 velocity = vec3(rand() % 2 / 10.0, -rand() % 2 / 10.0, rand() % 2 / 10.0);
	float size = rand() % 6 / 10.0 + 0.5;
	float mass = 1;
	vec3 color = vec3(rand() % 11 / 10.0, rand() % 11 / 10.0, rand() % 11 / 10.0);
	Sphere* sp = new Sphere(position, rotation, velocity, size, mass, color);
	sphere.push_back(sp);
}
void addCube(){
	//vec3 position = vec3(rand()%(gridSize-5)-2,begin_x+gridSize-4,rand()%(gridSize-5)-2);
	vec3 position = vec3((gridSize - 5) - 2, begin_x + gridSize - 4, (gridSize - 5) - 2);
	vec3 rotation = vec3(0, 0, 1);
	vec3 velocity = vec3(rand() % 4 / 15.0, -rand() % 4 / 15.0, rand() % 4 / 15.0);
	float size = 1;
	float mass = 1;
	vec3 color = vec3(rand() % 11 / 10.0, rand() % 11 / 10.0, rand() % 11 / 10.0);
	Cube *cube = new Cube(position, rotation, velocity, size, mass, color);
	cube->angularVelocity = vec3(rand() % 4 / 10.0, -rand() % 4 / 10.0, rand() % 4 / 10.0);
	c3.push_back(cube);
}
void addCylinder(){
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
void addPlane(){
	//top,bottom,left,right,front,back
	vec3 pos[6] = { vec3(1.5, begin_y + gridSize - 1.5, 1.5), vec3(1.5, begin_y + 1.5, 1.5), vec3(begin_x + 1.5, 1.5, 1.5),
		vec3(begin_x + gridSize - 1.5, 1.5, 1.5), vec3(1.5, 1.5, begin_z + gridSize - 1.5), vec3(1.5, 1.5, begin_z + 1.5) };
	vec3 rot[6] = { vec3(PI, 0, 0), vec3(0, 0, 0), vec3(0, 0, -PI / 2), vec3(0, 0, PI / 2), vec3(-PI / 2, 0, 0), vec3(PI / 2, 0, 0) };
	for (int i = 0; i < 6; i++){
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
void transparentPlane(){
	//top,bottom,left,right,front, back
	for (int i = 0; i < 6; i++)
	{
		if (plane[i]->color.a == 0.2f) plane[i]->color.a = 0.8f;
		else plane[i]->color.a = 0.2f;

	}
}

int lastKey1 = GLFW_RELEASE;
int lastKey2 = GLFW_RELEASE;
int lastKey3 = GLFW_RELEASE;
int lastKey4 = GLFW_RELEASE;
int lastKey5 = GLFW_RELEASE;
int lastKey6 = GLFW_RELEASE;
int lastKey7 = GLFW_RELEASE;
int lastKey8 = GLFW_RELEASE;
int lastKey9 = GLFW_RELEASE;
int lastMouse = GLFW_RELEASE;
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
class PickingRay
{
	vec3 clickPosInWorld;
	vec3 direction;

	/**
	* Computes the intersection of this ray with the X-Y Plane (where Z = 0)
	* and writes it back to the provided vector.
	*/
public:
	void intersectionWithXyPlane(vec3 worldPos)
	{
		float s = -clickPosInWorld.z / direction.z;
		worldPos[0] = clickPosInWorld.x + direction.x*s;
		worldPos[1] = clickPosInWorld.y + direction.y*s;
		worldPos[2] = 0;
	}

	vec3 getClickPosInWorld() {
		return clickPosInWorld;
	}
	vec3 getDirection() {
		return direction;
	}
};
/*
void picking(float screenX, float screenY, PickingRay pickingRay)
{
pickingRay.getClickPosInWorld().set(position);
pickingRay.getClickPosInWorld().add(view);

screenX -= (float)viewportWidth/2f;
screenY -= (float)viewportHeight/2f;

// normalize to 1
screenX /= ((float)viewportWidth/2f);
screenY /= ((float)viewportHeight/2f);

pickingRay.getClickPosInWorld().x += screenHoritzontally.x*screenX + screenVertically.x*screenY;
pickingRay.getClickPosInWorld().y += screenHoritzontally.y*screenX + screenVertically.y*screenY;
pickingRay.getClickPosInWorld().z += screenHoritzontally.z*screenX + screenVertically.z*screenY;

pickingRay.getDirection().set(pickingRay.getClickPosInWorld());
pickingRay.getDirection().sub(position);
}
*/
void pick(int mouse_x, int mouse_y){
	float x = (2.0f * mouse_x) / width - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / height;
	float z = 1.0f;
	vec3 ray_nds = vec3(x, y, z);
	vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	vec4 ray_eye = inverse(getProjectionMatrix()) * ray_clip;
	ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	vec3 ray_wor = (vec3)(inverse(getViewMatrix()) * ray_eye);
	// don't forget to normalise the vector at some point
	ray_wor = normalize(ray_wor);
	//cout<<"ray x= "<<ray_wor.x<<" y = "<<ray_wor.y<<" z = "<<ray_wor.z<<"\n";
}
void onPress(){
	//sphere
	if (glfwGetKey('1') == GLFW_PRESS){
		if (lastKey1 == GLFW_RELEASE) {
			addSphere();
		}
		lastKey1 = GLFW_PRESS;

	}
	else if (glfwGetKey('1') == GLFW_RELEASE){
		lastKey1 = GLFW_RELEASE;
	}

	//cube
	if (glfwGetKey('2') == GLFW_PRESS){
		if (lastKey2 == GLFW_RELEASE) addCube();
		lastKey2 = GLFW_PRESS;
	}
	else if (glfwGetKey('2') == GLFW_RELEASE){
		lastKey2 = GLFW_RELEASE;
	}

	//cylinder
	if (glfwGetKey('3') == GLFW_PRESS){
		if (lastKey3 == GLFW_RELEASE) addCylinder();
		lastKey3 = GLFW_PRESS;
	}
	else if (glfwGetKey('3') == GLFW_RELEASE){
		lastKey3 = GLFW_RELEASE;
	}

	//plane
	if (glfwGetKey('4') == GLFW_PRESS){
		if (lastKey4 == GLFW_RELEASE) transparentPlane();
		lastKey4 = GLFW_PRESS;
	}
	else if (glfwGetKey('4') == GLFW_RELEASE){
		lastKey4 = GLFW_RELEASE;
	}
	if (glfwGetKey('0') == GLFW_PRESS){
		if (lastKey5 == GLFW_RELEASE){
			if (pickObject) pickObject = 0;
			else pickObject = 1;
		}
		lastKey5 == GLFW_PRESS;
	}
	else if (glfwGetKey('0') == GLFW_RELEASE){
		lastKey5 = GLFW_RELEASE;
	}
	if (glfwGetKey('Z') == GLFW_PRESS){
		if (lastKey6 == GLFW_RELEASE)
		if (update) {
			update = 0;

		}
		else {
			update = 1;
			playFrame = 1;
		}
		lastKey6 = GLFW_PRESS;

	}
	else if (glfwGetKey('Z') == GLFW_RELEASE){
		lastKey6 = GLFW_RELEASE;

	}
	if (glfwGetKey('X') == GLFW_PRESS && !update){
		if (lastKey7 == GLFW_RELEASE){
			playFrame = 0;
			playOneFrame = 1;
		}
		lastKey7 = GLFW_PRESS;
	}
	else if (glfwGetKey('X') == GLFW_RELEASE){
		lastKey7 = GLFW_RELEASE;
	}
	if (glfwGetKey('C') == GLFW_PRESS){
		if (lastKey8 == GLFW_RELEASE){
			sphere.clear();
			c3.clear();
			cylinder.clear();
		}
		lastKey8 = GLFW_PRESS;
	}
	else if (glfwGetKey('C') == GLFW_RELEASE){
		lastKey8 = GLFW_RELEASE;
	}
	if (glfwGetKey('G') == GLFW_PRESS){
		if (lastKey9 == GLFW_RELEASE){
			if (enGravity) enGravity = 0;
			else enGravity = 1;
		}
		lastKey9 = GLFW_PRESS;
	}
	else if (glfwGetKey('G') == GLFW_RELEASE){
		lastKey9 = GLFW_RELEASE;
	}
	//pickBox=========================================================================================
	if (lastMouse == GLFW_RELEASE && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !pickObject){
		glfwGetMousePos(&clickX1, &clickY1);
		pick(clickX1, clickY1);
		lastMouse = GLFW_PRESS;
	}
	else if (lastMouse == GLFW_PRESS && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !pickObject) {
		glfwGetMousePos(&clickX2, &clickY2);
		float dx = clickX2 - clickX1;
		float dy = clickY2 - clickY1;
		clickX1 = clickX2;
		clickY1 = clickY2;
		dx /= 200.0f;
		dy /= -200.0f;
		//top,bottom,left,right,back,front
		float topPos = plane[0]->position.y;
		float btmPos = plane[1]->position.y;
		float lftPos = plane[2]->position.x;
		float rhtPos = plane[3]->position.x;
		//float bckPos = plane[4]->position.z;
		//float fntPos = plane[5]->position.z;
		vec4 pos = vec4(0, 0, 0, 0);
		if (topPos + dy <= begin_y + gridSize && btmPos + dy > begin_y) pos.y = dy;
		if (lftPos + dx >= begin_x && rhtPos + dx < begin_x + gridSize) pos.x = dx;
		grid.clearGridPlane();
		for (int i = 0; i < plane.size(); i++) {
			plane[i]->position += pos;
			grid.hashPlane(plane[i]);
		}

	}
	else if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && !pickObject) {
		lastMouse = GLFW_RELEASE;
	}

	//pickObject=========================================================================================
	if (lastMouse == GLFW_RELEASE && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && pickObject){
		glfwGetMousePos(&clickX1, &clickY1);
		lastMouse = GLFW_PRESS;
	}
	else if (lastMouse == GLFW_PRESS && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && pickObject) {

	}
	else if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && pickObject) {
		lastMouse = GLFW_RELEASE;
	}

}

int main(void)
{
	cout << "start\n";
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	cout << "init\n";
	//GLUTkeyboardfun(keyboard);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
	// Open a window and create its OpenGL context
	if (!glfwOpenWindow(1024, 768, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
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
	// Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID2, "vertexPosition_modelspace");
	GLuint vertexRotation_modelspaceID = glGetAttribLocation(programID2, "vertexRotation_modelspace");
	GLuint vertexUVID = glGetAttribLocation(programID2, "vertexUV");
	GLuint vertexNormal_modelspaceID = glGetAttribLocation(programID2, "vertexNormal_modelspace");


	//---------------------------------------------------------------------------------------------------------------

	//addCube();
	//addSphere();
	addPlane();/*
			   addCylinder();
			   addCylinder();
			   addCylinder();
			   Cylinder* cyl = cylinder[0];
			   cyl->velocity = vec4(0,0,0,0);
			   cyl->position = vec4(0,-3.5,0,1);
			   cyl->orientation = vec3(0,0,0);
			   cyl->angularVelocity = vec3(1,0,0);
			   cyl->color = vec4(1,0,0,1);

			   Cylinder* cyl2 = cylinder[1];
			   cyl2->velocity = vec4(0,0,0,0);
			   cyl2->position = vec4(2,-3.5,0,1);
			   cyl2->orientation = vec3(0,0,0);
			   cyl2->angularVelocity = vec3(0,1,0);
			   cyl2->color = vec4(0,1,0,1);

			   Cylinder* cyl3 = cylinder[2];
			   cyl3->velocity = vec4(0,0,0,0);
			   cyl3->position = vec4(0,-3.5,2,1);
			   cyl3->orientation = vec3(0,0,0);
			   cyl3->angularVelocity = vec3(0,0,1);
			   cyl3->color = vec4(0,0,1,1);*/
	grid = Grid(plane);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	GLuint rotatebuffer;
	glGenBuffers(1, &rotatebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rotatebuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_rotates.size() * sizeof(glm::mat4), &indexed_rotates[0], GL_STATIC_DRAW);
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID2);
	GLuint LightID = glGetUniformLocation(programID2, "LightPosition_worldspace");

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	do{
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID2);

		// Compute the MVP matrix from keyboard and mouse input
		grid.hashGrid(c3, cylinder, sphere);
		if (update || playOneFrame){
			grid.checkCollisionGrid();
		}
		computeMatricesFromInputs();
		grid.clearGrid();
		onPress();
		glm::mat4 ModelMatrix = mat4(1.0f);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		for (int i = 0; i < c3.size(); i++)
		{
			if (update || playOneFrame){
				c3[i]->updatePosition(timeStep, GRAVITY*enGravity);
				c3[i]->setEdge();
			}
			glm::mat4 ScaleMatrix = mat4();
			glm::mat4 RotateMatrix = (*c3[i]).getRotationMatrix();
			glm::mat4 TranslateMatrix = (*c3[i]).getTranslationMatrix();
			glm::mat4 RotateModel = mat4(1);
			glm::mat4 TranslateModel = mat4(1);
			glPushMatrix();
			glUniformMatrix4fv(ScaleMatrixID, 1, GL_FALSE, &ScaleMatrix[0][0]);
			glUniformMatrix4fv(RotateMatrixID, 1, GL_FALSE, &RotateMatrix[0][0]);
			glUniformMatrix4fv(TranslateMatrixID, 1, GL_FALSE, &TranslateMatrix[0][0]);
			glUniformMatrix4fv(TranslateModelID, 1, GL_FALSE, &TranslateModel[0][0]);
			glUniformMatrix4fv(RotateModelID, 1, GL_FALSE, &RotateModel[0][0]);
			c3[i]->render();
			glPopMatrix();
		}
		for (int i = 0; i < sphere.size(); i++)
		{
			if (update || playOneFrame)(*sphere[i]).updatePosition(timeStep, GRAVITY*enGravity);
			glm::mat4 ScaleMatrix = mat4();
			glm::mat4 RotateMatrix = (*sphere[i]).getRotationMatrix();
			glm::mat4 TranslateMatrix = (*sphere[i]).getTranslationMatrix();
			glm::mat4 RotateModel = mat4(1);
			glm::mat4 TranslateModel = mat4(1);
			glPushMatrix();
			glUniformMatrix4fv(ScaleMatrixID, 1, GL_FALSE, &ScaleMatrix[0][0]);
			glUniformMatrix4fv(RotateMatrixID, 1, GL_FALSE, &RotateMatrix[0][0]);
			glUniformMatrix4fv(TranslateMatrixID, 1, GL_FALSE, &TranslateMatrix[0][0]);
			glUniformMatrix4fv(TranslateModelID, 1, GL_FALSE, &TranslateModel[0][0]);
			glUniformMatrix4fv(RotateModelID, 1, GL_FALSE, &RotateModel[0][0]);
			(*sphere[i]).render();
			glPopMatrix();
		}
		for (int i = 0; i < cylinder.size(); i++)
		{
			if (update || playOneFrame)(*cylinder[i]).updatePosition(timeStep, GRAVITY*enGravity);
			glm::mat4 ScaleMatrix = mat4();
			glm::mat4 RotateMatrix = cylinder[i]->getRotationMatrix();
			glm::mat4 TranslateMatrix = cylinder[i]->getTranslationMatrix();
			glm::mat4 RotateModel = cylinder[i]->getRotationMatrixRender();
			glm::mat4 TranslateModel = cylinder[i]->getTranslationMatrixRender();
			glPushMatrix();
			glUniformMatrix4fv(ScaleMatrixID, 1, GL_FALSE, &ScaleMatrix[0][0]);
			glUniformMatrix4fv(RotateMatrixID, 1, GL_FALSE, &RotateMatrix[0][0]);
			glUniformMatrix4fv(TranslateMatrixID, 1, GL_FALSE, &TranslateMatrix[0][0]);
			glUniformMatrix4fv(TranslateModelID, 1, GL_FALSE, &TranslateModel[0][0]);
			glUniformMatrix4fv(RotateModelID, 1, GL_FALSE, &RotateModel[0][0]);
			(*cylinder[i]).render();
			glPopMatrix();
		}
		for (int i = 0; i < plane.size(); i++)
		{
			if (update || playOneFrame)(*plane[i]).updatePosition(0.01f, 0);
			glm::mat4 ScaleMatrix = mat4();
			glm::mat4 RotateMatrix = (*plane[i]).getRotationMatrix();
			glm::mat4 TranslateMatrix = (*plane[i]).getTranslationMatrix();
			glm::mat4 RotateModel = mat4(1);
			glm::mat4 TranslateModel = mat4(1);
			glPushMatrix();
			glUniformMatrix4fv(ScaleMatrixID, 1, GL_FALSE, &ScaleMatrix[0][0]);
			glUniformMatrix4fv(RotateMatrixID, 1, GL_FALSE, &RotateMatrix[0][0]);
			glUniformMatrix4fv(TranslateMatrixID, 1, GL_FALSE, &TranslateMatrix[0][0]);
			glUniformMatrix4fv(TranslateModelID, 1, GL_FALSE, &TranslateModel[0][0]);
			glUniformMatrix4fv(RotateModelID, 1, GL_FALSE, &RotateModel[0][0]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_SMOOTH);
			plane[i]->render();
			glPopMatrix();
		}
		playOneFrame = 0;
		glm::mat4 ScaleMatrix = mat4();
		glm::mat4 RotateMatrix = mat4();
		glm::mat4 TranslateMatrix = mat4();
		glm::mat4 RotateModel = mat4(1);
		glm::mat4 TranslateModel = mat4(1);
		glUniformMatrix4fv(ScaleMatrixID, 1, GL_FALSE, &ScaleMatrix[0][0]);
		glUniformMatrix4fv(RotateMatrixID, 1, GL_FALSE, &RotateMatrix[0][0]);
		glUniformMatrix4fv(TranslateMatrixID, 1, GL_FALSE, &TranslateMatrix[0][0]);
		glUniformMatrix4fv(TranslateModelID, 1, GL_FALSE, &TranslateModel[0][0]);
		glUniformMatrix4fv(RotateModelID, 1, GL_FALSE, &RotateModel[0][0]);
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);


		//1rst attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
			);
		glEnableVertexAttribArray(vertexRotation_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, rotatebuffer);
		glVertexAttribPointer(
			vertexRotation_modelspaceID,  // The attribute we want to configure
			16,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
			);
		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(vertexUVID);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			vertexUVID,                   // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
			);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(vertexNormal_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			vertexNormal_modelspaceID,    // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
			);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			indices.size(),    // count
			GL_UNSIGNED_SHORT, // type
			(void*)0           // element array buffer offset
			);

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		glDisableVertexAttribArray(vertexRotation_modelspaceID);
		glDisableVertexAttribArray(vertexUVID);
		glDisableVertexAttribArray(vertexNormal_modelspaceID);

		//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		/*glColor3f(0.0f,0.5f,0.0f);
		gluCylinder(gluNewQuadric(),0.1,0.1,1,20,2);
		GLUquadric* sphere;
		sphere=gluNewQuadric();
		gluQuadricNormals(sphere, GL_SMOOTH);
		glColor3f(1,0,0);
		gluSphere(sphere,0.2,10,10);*/
		// Swap buffers
		glfwSwapBuffers();
		playOneFrame = 0;
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
	glfwGetWindowParam(GLFW_OPENED));

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &rotatebuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(programID2);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

