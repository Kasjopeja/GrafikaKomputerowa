#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"



const unsigned int width = 800;
const unsigned int height = 800;



// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      //
	 -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Front-bottom-left
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // Front-bottom-right
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // Front-top-right
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // Front-top-left

	// Back face
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // Back-bottom-left
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // Back-bottom-right
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // Back-top-right
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // Back-top-left

	// Left face
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Front-top-left
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // Back-top-left
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, // Back-bottom-left
	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // Front-bottom-left

	// Right face
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // Front-top-right
	 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // Back-top-right
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // Back-bottom-right
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, // Front-bottom-right

	 // Top face
	 -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // Back-top-left
	 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // Front-top-left
	  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // Front-top-right
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, // Back-top-right

	  // Bottom face
	  -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // Back-bottom-left
	  -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // Front-bottom-left
	   0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // Front-bottom-right
	   0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, // Back-bottom-right
};

// Indices for vertices order
GLuint indices[] =
{
	// Front face
	0, 1, 2,   0, 2, 3,
	// Back face
	4, 5, 6,   4, 6, 7,
	// Left face
	8, 9, 10,  8, 10, 11,
	// Right face
	12, 13, 14, 12, 14, 15,
	// Top face
	16, 17, 18, 16, 18, 19,
	// Bottom face
	20, 21, 22, 20, 22, 23
};


int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");


	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// Binds texture so that is appears in rendering
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}


	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}