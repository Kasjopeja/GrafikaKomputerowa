#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO& VBO, GLuint posisionLocation, GLuint colorLocation)
{
	VBO.Bind();
	glVertexAttribPointer(posisionLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(posisionLocation);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(colorLocation);
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}