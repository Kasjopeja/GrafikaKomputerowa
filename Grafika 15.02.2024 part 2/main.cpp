#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

using namespace std;

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"in vec2 position;\n"
"in vec3 color;\n"
"out vec3 Color;\n"
"void main()\n"
"{\n"
" Color = color;\n"
" gl_Position = vec4(position, 0.0, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 Color;\n"
"out vec4 outColor;\n"	
"void main()\n"
"{\n"
" outColor = vec4(Color, 1.0);\n"
"}\n\0";

int main() {

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

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);


	GLuint VAO;

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Vertices coordinates
	GLfloat vertices[] =
	{
	-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Lower left corner
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Lower right corner
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Upper right corner
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f // Upper left corner
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// za�aduj przygotowane wierzcho�ki
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint vStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vStatus);
	cout << "Status kompilacji Vertex Shadera: ";
	if (vStatus == GL_TRUE) cout << "RIGHT COMPILATION" << endl << endl;
	else {
		cout << "BAD COMPILATION" << endl;
		char vBuffer[128];
		glGetShaderInfoLog(vertexShader, 64, NULL, vBuffer);
		cout << "Log Kompilacji Vertex Shadera: " << endl << vBuffer << endl << endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);



	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glUseProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));


	// Specify the color of the background
	glClearColor(0.01f, 0.01f, 0.01f, 0.01f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window))
	{
		// Ustaw kolor t�a (RGBA, z przedzia�u <0, 1>)
		glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
		// Wyczy�� buffor I nadaj mu kolor
		glClear(GL_COLOR_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GL_UNSIGNED_INT)));


		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();


	return 0;
}