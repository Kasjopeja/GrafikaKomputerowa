#include"Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
    // Assigns the type of the texture to the texture object
    type = texType;

    // Stores the width, height, and the number of color channels of the image
    int widthImg, heightImg, numColCh;
    // Flips the image so it appears right side up
    stbi_set_flip_vertically_on_load(true);
    // Reads the image from a file and stores it in bytes
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

    if (!bytes) {
        std::cerr << "Failed to load texture: " << image << std::endl;
        return;
    }

    std::cout << "Loaded texture successfully!" << std::endl;
    std::cout << "Width: " << widthImg << ", Height: " << heightImg << ", Channels: " << numColCh << std::endl;

    // Check if the dimensions exceed the maximum texture size supported by the GPU
        GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    if (widthImg > maxTextureSize || heightImg > maxTextureSize) {
        std::cerr << "Texture size exceeds maximum supported size of " << maxTextureSize << "x" << maxTextureSize << std::endl;
        stbi_image_free(bytes);
        return;
    }

    // Generates an OpenGL texture object
    glGenTextures(1, &ID);
    // Assigns the texture to a Texture Unit
    glActiveTexture(slot);
    glBindTexture(texType, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Assigns the image to the OpenGL Texture object
    glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    // Generates MipMaps
    glGenerateMipmap(texType);

    // Check for any OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }

    // Deletes the image data as it is already in the OpenGL Texture object
    stbi_image_free(bytes);

    // Unbinds the OpenGL Texture object so that it can't accidentally be modified
    glBindTexture(texType, 0);
}
void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}