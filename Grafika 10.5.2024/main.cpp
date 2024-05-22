#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Vertices coordinates
GLfloat vertices[] = {
    // Positions          // Colors          // Texture Coords
    // Front face
    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top-right
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, // Top-left

    // Right face
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // Top-left

     // Back face
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom-left
     -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom-right
     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top-right
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // Top-left

      // Left face
      -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom-left
      -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom-right
      -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top-right
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // Top-left

      // Top face
      -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom-left
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom-right
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top-right
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // Top-left

      // Bottom face
      -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top-right
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // Top-left
       0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom-left
      -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f  // Bottom-right
};

// Mirror vertices (if different texture or properties required)
GLfloat mirrorVertices[] = {
    // Positions          // Colors          // Texture Coords
    -1.0f, -0.5f, -1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f, // Top-right
     1.0f, -0.5f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // Top-left
     1.0f, -0.5f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom-left
    -1.0f, -0.5f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f  // Bottom-right
};

// Indices for vertices order
GLuint indices[] = {
    // Front face
    0, 1, 2,    0, 2, 3,
    // Right face
    4, 5, 6,    4, 6, 7,
    // Back face
    8, 9, 10,   8, 10, 11,
    // Left face
    12, 13, 14, 12, 14, 15,
    // Top face
    16, 17, 18, 16, 18, 19,
    // Bottom face
    20, 21, 22, 20, 22, 23
};

GLuint indicesMirror[] = {
    0, 1, 2, 0, 2, 3
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

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Project", NULL, NULL);
    // Error check if the window fails to create
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

    // Load GLAD so it configures OpenGL
    gladLoadGL();

    // Specify the viewport of OpenGL in the window
    // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
    glViewport(0, 0, width, height);

    // Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // Links VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Unbind all to prevent accidental modification
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Mirror setup
    VAO VAO2;
    VAO2.Bind();

    VBO VBO2(mirrorVertices, sizeof(mirrorVertices));
    EBO EBO2(indicesMirror, sizeof(indicesMirror));

    VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO2.LinkAttrib(VBO2, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    VAO2.Unbind();
    VBO2.Unbind();
    EBO2.Unbind();

    // Texture
    Texture diamondTex("diamond_block.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    // Camera object
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);

        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();

        // Get current time
        float currentTime = glfwGetTime();

        // Handle camera inputs
        camera.Inputs(window);

        // Update and export the camera matrix to the Vertex Shader
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        GLint uniColor = glGetUniformLocation(shaderProgram.ID, "overrideColor");

        // Bind the texture
        diamondTex.Bind();

        // ---------------------------------------------------------------------------------------------------------
        // Bind the VAO1 so OpenGL knows to use it
        VAO1.Bind();
        // Set the model matrix for the first cube
        glm::mat4 model1 = glm::mat4(1.0f);
        GLuint modelLoc1 = glGetUniformLocation(shaderProgram.ID, "modelMatrix");
        glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
        // Draw the first cube
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        glEnable(GL_STENCIL_TEST);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF);
        glDepthMask(GL_FALSE);
        glClear(GL_STENCIL_BUFFER_BIT);

        // ---------------------------------------------------------------------------------------------------------
        // Bind the VAO2 so OpenGL knows to use it
        VAO2.Bind();
        // Set the model matrix for the mirror
        glm::mat4 modelMirror = glm::mat4(1.0f);
        GLuint modelMirrorLoc = glGetUniformLocation(shaderProgram.ID, "modelMatrix");
        glUniformMatrix4fv(modelMirrorLoc, 1, GL_FALSE, glm::value_ptr(modelMirror));
        // Draw the mirror
        glDrawElements(GL_TRIANGLES, sizeof(indicesMirror) / sizeof(int), GL_UNSIGNED_INT, 0);

        // ---------------------------------------------------------------------------------------------------------
        // Bind the VAO1 so OpenGL knows to use it
        VAO1.Bind();
        // Set the model matrix for the second cube
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(0.0f, -1.0f, 0.0f)); // Translate the second cube
        model2 = glm::scale(model2, glm::vec3(-1.0f, -1.0f, 1.0f));    // Mirror the second cube
        GLuint modelLoc2 = glGetUniformLocation(shaderProgram.ID, "modelMatrix");
        glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));

        glStencilFunc(GL_EQUAL, 1, 0Xff);
        glStencilMask(0x00);
        glDepthMask(GL_TRUE);

        glUniform3f(uniColor, 0.4f, 0.4f, 0.4f);
        // Draw the second cube
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);

        glDisable(GL_STENCIL_TEST);

        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }


    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();

    VAO2.Delete();
    VBO2.Delete();
    EBO2.Delete();

    diamondTex.Delete();
    shaderProgram.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}
