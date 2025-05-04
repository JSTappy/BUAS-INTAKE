#include <iostream> 
#include "config.h"
#include "camera.h"

Camera::Camera()
{
	_projectionMatrix = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, 0.1f, 100.0f);
	_viewMatrix = glm::mat4(1.0f);
	_cursor = glm::vec3( 0, 0, 0 );
}

Camera::~Camera()
{
	
}

void Camera::computeViewMatrixFromInput(GLFWwindow* window, float deltaTime)
{
	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	_cursor = glm::vec3( xpos, ypos, 0 );

	// Right and Up vector
	glm::vec3 right = glm::vec3(1, 0, 0);
	glm::vec3 up = glm::vec3(0, -1, 0);

	static glm::vec3 position = glm::vec3( 0, 0, 10 ); // Initial position : on +Z
	float speed = 300.0f; // units / second

	// Move up
	//if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
	//	position += up * deltaTime * speed;
	//	// std::cout << "Moving Up" << std::endl;
	//}
	//// Move down
	//if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
	//	position -= up * deltaTime * speed;
	//	// std::cout << "Moving Down" << std::endl;

	//}
	//// Strafe right
	//if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
	//	position += right * deltaTime * speed;
	//	// std::cout << "Moving Right" << std::endl;

	//}
	//// Strafe left
	//if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
	//	position -= right * deltaTime * speed;
	//	// std::cout << "Moving Left" << std::endl;

	//}

	// View matrix
	_viewMatrix = glm::lookAt(
			position, /* Camera is at (xpos,ypos,zpos), in World Space */
			position + glm::vec3(0, 0, -1), /* and looks towards Z */
			glm::vec3(0, 1, 0)  /* Head is up (set to 0,-1,0 to look upside-down) */
		);
}
