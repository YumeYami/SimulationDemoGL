// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <iostream>
#include "controls.hpp"
#include "demo.h"
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

vec3 getPosView(){
	return vec3();
}
using namespace std;

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 5 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;
int fixXpos=512,fixYpos=384;
int showXpos=512,showYpos=384;
int clickXpos=0,clickYpos=0;
int xpos,ypos;
int lastMouseRightStatus=GLFW_RELEASE;
//temp key
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



void computeMatricesFromInputs(){
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS && lastMouseRightStatus==GLFW_RELEASE){
		glfwGetMousePos(&clickXpos,&clickYpos);
		lastMouseRightStatus=GLFW_PRESS;
	}
	else if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS && lastMouseRightStatus==GLFW_PRESS){
		glfwGetMousePos(&xpos,&ypos);
		lastMouseRightStatus=GLFW_PRESS;
		showXpos=xpos+fixXpos-clickXpos;
		showYpos=ypos+fixYpos-clickYpos;
		//cout<<xpos<<" "<<fixXpos<<"  "<<clickXpos<<endl;
	}
	else if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)==GLFW_RELEASE && lastMouseRightStatus==GLFW_PRESS) {
		lastMouseRightStatus=GLFW_RELEASE;
		fixXpos=showXpos;
		fixYpos=showYpos;
	}
	// Reset mouse position for next frame
	// EDIT : Doesn't work on Mac OS, hence the code below is a bit different from the websites
	//glfwSetMousePos(1024/2, 768/2);

	// Compute new orientation
	horizontalAngle = 3.14f + mouseSpeed * float(1024/2 - showXpos);
	verticalAngle   = mouseSpeed * float( 768/2 - showYpos);


	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
		);

	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey('W') == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey('S') == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey('D') == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey('A') == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}
	if (glfwGetKey('Q') == GLFW_PRESS){
		position += up * deltaTime * speed;
	}
	if (glfwGetKey('E') == GLFW_PRESS){
		position -= up * deltaTime * speed;
	}
	float FoV = initialFoV - 5 * glfwGetMouseWheel();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
		position,           // Camera is here
		position+direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
//
void pick(int mouse_x, int mouse_y) {
	float x = (2.0f * mouse_x) / 512 - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / 384;
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
void onPress() {
	//sphere
	if ( glfwGetKey('1') == GLFW_PRESS ) {
		if ( lastKey1 == GLFW_RELEASE ) {
			addSphere();
		}
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
	//if ( glfwGetKey('0') == GLFW_PRESS ) {
	//	if ( lastKey5 == GLFW_RELEASE ) {
	//		if ( pickObject ) pickObject = 0;
	//		else pickObject = 1;
	//	}
	//	lastKey5 == GLFW_PRESS;
	//}
	//else if ( glfwGetKey('0') == GLFW_RELEASE ) {
	//	lastKey5 = GLFW_RELEASE;
	//}
	//if ( glfwGetKey('Z') == GLFW_PRESS ) {
	//	if ( lastKey6 == GLFW_RELEASE )
	//	if ( update ) {
	//		update = 0;
	//	}
	//	else {
	//		update = 1;
	//		playFrame = 1;
	//	}
	//	lastKey6 = GLFW_PRESS;
	//}
	//else if ( glfwGetKey('Z') == GLFW_RELEASE ) {
	//	lastKey6 = GLFW_RELEASE;
	//}
	//if ( glfwGetKey('X') == GLFW_PRESS && !update ) {
	//	if ( lastKey7 == GLFW_RELEASE ) {
	//		playFrame = 0;
	//		playOneFrame = 1;
	//	}
	//	lastKey7 = GLFW_PRESS;
	//}
	//else if ( glfwGetKey('X') == GLFW_RELEASE ) {
	//	lastKey7 = GLFW_RELEASE;
	//}
	//if ( glfwGetKey('C') == GLFW_PRESS ) {
	//	if ( lastKey8 == GLFW_RELEASE ) {
	//		sphere.clear();
	//		c3.clear();
	//		cylinder.clear();
	//	}
	//	lastKey8 = GLFW_PRESS;
	//}
	//else if ( glfwGetKey('C') == GLFW_RELEASE ) {
	//	lastKey8 = GLFW_RELEASE;
	//}
	//if ( glfwGetKey('G') == GLFW_PRESS ) {
	//	if ( lastKey9 == GLFW_RELEASE ) {
	//		if ( enGravity ) enGravity = 0;
	//		else enGravity = 1;
	//	}
	//	lastKey9 = GLFW_PRESS;
	//}
	//else if ( glfwGetKey('G') == GLFW_RELEASE ) {
	//	lastKey9 = GLFW_RELEASE;
	//}
	////pickBox=========================================================================================
	//if ( lastMouse == GLFW_RELEASE && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !pickObject ) {
	//	glfwGetMousePos(&clickX1, &clickY1);
	//	pick(clickX1, clickY1);
	//	lastMouse = GLFW_PRESS;
	//}
	//else if ( lastMouse == GLFW_PRESS && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !pickObject ) {
	//	glfwGetMousePos(&clickX2, &clickY2);
	//	float dx = clickX2 - clickX1;
	//	float dy = clickY2 - clickY1;
	//	clickX1 = clickX2;
	//	clickY1 = clickY2;
	//	dx /= 200.0f;
	//	dy /= -200.0f;
	//	float topPos = plane[0]->position.y;
	//	float btmPos = plane[1]->position.y;
	//	float lftPos = plane[2]->position.x;
	//	float rhtPos = plane[3]->position.x;
	//	vec4 pos = vec4(0, 0, 0, 0);
	//	if ( topPos + dy <= begin_y + gridSize && btmPos + dy > begin_y ) pos.y = dy;
	//	if ( lftPos + dx >= begin_x && rhtPos + dx < begin_x + gridSize ) pos.x = dx;
	//	grid.clearGridPlane();
	//	for ( int i = 0; i < plane.size(); i++ ) {
	//		plane[i]->position += pos;
	//		grid.hashPlane(plane[i]);
	//	}
	//}
	//else if ( glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && !pickObject ) {
	//	lastMouse = GLFW_RELEASE;
	//}
	////pickObject=========================================================================================
	//if ( lastMouse == GLFW_RELEASE && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && pickObject ) {
	//	glfwGetMousePos(&clickX1, &clickY1);
	//	lastMouse = GLFW_PRESS;
	//}
	//else if ( lastMouse == GLFW_PRESS && glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && pickObject ) {
	//}
	//else if ( glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && pickObject ) {
	//	lastMouse = GLFW_RELEASE;
	//}
}