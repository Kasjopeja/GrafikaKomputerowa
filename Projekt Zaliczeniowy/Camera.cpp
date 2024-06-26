#include"Camera.h"
Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}
void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader,
	const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height,
		nearPlane, farPlane);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE,
		glm::value_ptr(projection * view));
}

void Camera::Follow(glm::vec3 targetPosition)
{
	Position = targetPosition + glm::vec3(0.0f, 5.0f, 5.0f); // Adjust offset as needed
	Orientation = glm::normalize(targetPosition - Position);
}

void Camera::Inputs(GLFWwindow* window, glm::vec3 planetPositions[])
{
	float speed = 0.01f;

	// Obsluga klawiszy
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
		//PrintPosition();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
		//PrintPosition();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
		//PrintPosition();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
		//PrintPosition();
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		Position = glm::vec3(0.0f, 10.0f, 10.0f);
		Orientation = glm::vec3(0.0f, -1.0f, -1.0f);
	}

	// Handle camera follow keys (1 to 9)
	for (int i = GLFW_KEY_1; i <= GLFW_KEY_9; i++)
	{
		if (glfwGetKey(window, i) == GLFW_PRESS)
		{
			Follow(planetPositions[i - GLFW_KEY_1]);
		}
	}

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);
		// Normalizes and shifts the coordinates of the cursor such that they
		//begin in the middle of the screen
			// and then �transforms� them into degrees
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;
		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-
			rotX), glm::normalize(glm::cross(Orientation, Up)));
		// Decides whether or not the next vertical Orientation is legal or
		//not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <=
			glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}
		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
		// Sets mouse cursor to the middle of the screen so that it doesn�t
		//end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn�t jump
		firstClick = true;
	}
}
