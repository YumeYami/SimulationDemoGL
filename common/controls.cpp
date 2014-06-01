// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include <iostream>
#include "controls.hpp"

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
	// EDIT : Doesn't work on Mac OS, hence the code below is a bit different from the website's
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