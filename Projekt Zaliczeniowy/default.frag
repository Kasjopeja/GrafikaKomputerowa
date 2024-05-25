#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the current position from the Vertex Shader
in vec3 crntPos;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;
uniform sampler2D tex1;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.20f;
    vec3 ambient = ambientStrength * lightColor.rgb;

    // Diffuse lighting
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);
    float diff = max(dot(normal, lightDirection), 0.0f);
    vec3 diffuse = diff * lightColor.rgb;

    // Specular lighting
    float specularStrength = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    vec3 specular = specularStrength * spec * lightColor.rgb;

    // Combine all three lighting components
    vec3 lighting = (ambient + diffuse + specular);

    // Apply texture color
    vec4 texColor = texture(tex0, texCoord);

    // Final color output
    FragColor = 1.5 * vec4(texColor.rgb * lighting, texColor.a);
}
