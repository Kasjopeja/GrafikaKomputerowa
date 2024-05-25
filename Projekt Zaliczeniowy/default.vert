#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Texture Coordinates
layout (location = 3) in vec2 aTex;
// Colors
layout (location = 2) in vec3 aColor;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;


// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the current position for the Fragment Shader
out vec3 crntPos;

// Imports the camera matrix from the main function
uniform mat4 camMatrix;
// Imports the model matrix from the main function
uniform mat4 modelMatrix;

void main()
{

    // calculates current position
	crntPos = vec3(modelMatrix * vec4(aPos, 1.0f));
    // Outputs the positions/coordinates of all vertices
    gl_Position = camMatrix * modelMatrix * vec4(aPos, 1.0);

    // Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = aTex;
	 // Transform the normal by the inverse transpose of the model matrix
    Normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
}
