#version 330 core
in ver3 Color;
out vec4 FragColor;

void main()
{
	FragColor = vec4(color, 1.0f);
}