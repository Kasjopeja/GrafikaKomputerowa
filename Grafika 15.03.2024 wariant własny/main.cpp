#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

using namespace std;

int main()
{
	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Openglwindow", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" <<
			std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport 
	// of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);
	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	// Vertex Shader source code
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	//Fragment Shader source code
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		" FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
		"}\n\0";

	// Utwórz obiekt Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Połącz istniejący obiekt z napisaną wcześniej implementacją shadera
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Skompiluj gotowy kod
	glCompileShader(vertexShader);
	// Powtórz operację dla fragment shadera
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Utwórz program
	GLuint shaderProgram = glCreateProgram();
	// Podłącz shadery pod program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Usuń niepotrzebne shadery
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertices coordinates
	vector <GLfloat> vertices;
	
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 11; i++)
		{
			vertices.push_back(-1.0 + (0.8 * j) + (((i + 1) % 2) * 0.4));
			vertices.push_back(1.0 - (0.4 * ceil(double(i) / 2)));
		}
	}

	for (int i = 0; i < vertices.size(); i = i + 2)
	{
		cout << vertices[i] << " " << vertices[i + 1] << endl;
	}

	// Indices for vertices order
	vector <GLuint> indices = { 0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 13, 14, 15, 15, 16, 17, 17, 18, 19, 19, 20, 21, 22, 23, 24, 24, 25, 26, 26, 27, 28, 28, 29, 30, 30, 31, 32};

	vector<GLuint> sequence;

	for (int i = 0; i <= 32; i++) {
		if (i % 4 == 0) {
			// Every fourth number (starting from 0) is not repeated
			// It's the beginning of a 'zag'
			sequence.push_back(i);
		}
		else if ((i + 1) % 4 == 0) {
			// Every fourth number (offset by 1) is the end of a 'zig'
			// and is not repeated
			sequence.push_back(i);
			sequence.push_back(i + 1); // Begin next 'zag'
		}
		else {
			// Middle number of a 'zig' or first number of a 'zag'
			sequence.push_back(i);
			sequence.push_back(i);
		}
	}

	// Output the sequence
	for (int num : sequence) {
		cout << num << ", ";
	}
	cout << endl;

	GLsizei elemSize = indices.size() * sizeof(GLuint);

	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, elemSize, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();


	return 0;
}