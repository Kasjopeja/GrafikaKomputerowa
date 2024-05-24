#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include <vector>

#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "Sphere.h"

int main()
{
    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we are using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Get the primary monitor
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    // Get the video mode of the primary monitor
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    // Create a GLFWwindow object in fullscreen mode
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "OpenGL Project", primaryMonitor, NULL);

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
    glViewport(0, 0, mode->width, mode->height);

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");

    // Camera object
    Camera camera(mode->width, mode->height, glm::vec3(0.0f, 10.0f, 10.0f));

    // Define array of planet positions for camera follow
    glm::vec3 planetPositions[9];

    // Texture
    Texture skyboxTex("skybox.jpeg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture diamondTex("diamond_block.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture sunTex("planets/2k_sun.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    Texture mercuryTex("planets/2k_mercury.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture venusTex("planets/2k_venus.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture earthTex("planets/earth2k.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture marsTex("planets/2k_mars.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture jupiterTex("planets/2k_jupiter.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture saturnTex("planets/2k_saturn.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture uranusTex("planets/2k_uranus.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture neptuneTex("planets/2k_neptune.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    Texture moonTex("planets/Moon_texture.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    Sphere skybox(shaderProgram, skyboxTex, 50.0f, 36, 18);
    glm::mat4 skyboxModel = glm::mat4(1.0f);

    // Create spheres
    Sphere sun(shaderProgram, sunTex, 0.5f, 36, 18);
    Sphere mercury(shaderProgram, mercuryTex, 0.1f, 36, 18);
    Sphere venus(shaderProgram, venusTex, 0.15f, 36, 18);
    Sphere earth(shaderProgram, earthTex, 0.2f, 36, 18);
    Sphere mars(shaderProgram, marsTex, 0.25f, 36, 18);
    Sphere jupiter(shaderProgram, jupiterTex, 0.4f, 36, 18);
    Sphere saturn(shaderProgram, saturnTex, 0.4f, 36, 18);
    Sphere uranus(shaderProgram, uranusTex, 0.3f, 36, 18);
    Sphere neptune(shaderProgram, neptuneTex, 0.3f, 36, 18);
    Sphere moon(shaderProgram, moonTex, 0.05f, 36, 18); 

    // Initialize variables for orbit and self-rotation
    float mercuryOrbitAngle = 0.0f, venusOrbitAngle = 0.0f, earthOrbitAngle = 0.0f, marsOrbitAngle = 0.0f, jupiterOrbitAngle = 0.0f, saturnOrbitAngle = 0.0f, uranusOrbitAngle = 0.0f, neptuneOrbitAngle = 0.0f;
    float mercurySelfAngle = 0.0f, venusSelfAngle = 0.0f, earthSelfAngle = 0.0f, marsSelfAngle = 0.0f, jupiterSelfAngle = 0.0f, saturnSelfAngle = 0.0f, uranusSelfAngle = 0.0f, neptuneSelfAngle = 0.0f;
    float moonOrbitAngle = 0.0f, moonSelfAngle = 0.0f; 
    float mercuryOrbitSpeed = 1.0f, venusOrbitSpeed = 0.8f, earthOrbitSpeed = 0.5f, marsOrbitSpeed = 0.3f, jupiterOrbitSpeed = 0.2f, saturnOrbitSpeed = 0.1f, uranusOrbitSpeed = 0.05f, neptuneOrbitSpeed = 0.02f;
    float mercurySelfSpeed = 1.5f, venusSelfSpeed = 1.4f, earthSelfSpeed = 1.3f, marsSelfSpeed = 1.2f, jupiterSelfSpeed = 1.1f, saturnSelfSpeed = 1.0f, uranusSelfSpeed = 0.9f, neptuneSelfSpeed = 0.8f;
    float moonOrbitSpeed = 2.0f, moonSelfSpeed = 1.6f; 
    float mercuryDistance = 3.0f, venusDistance = 5.0f, earthDistance = 7.0f, marsDistance = 9.0f, jupiterDistance = 12.0f, saturnDistance = 15.0f, uranusDistance = 18.0f, neptuneDistance = 20.0f;
    float moonDistance = 0.6f; 


   
    // Main loop
    while (!glfwWindowShouldClose(window))
    {

        // Input handling
        camera.Inputs(window, planetPositions);

        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        // Update camera matrix
        camera.Matrix(50.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        skybox.Draw(skyboxModel);

        glm::mat4 sunModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        sun.Draw(sunModel);

        // Update angles for orbit and self-rotation
        mercuryOrbitAngle += mercuryOrbitSpeed; mercurySelfAngle += mercurySelfSpeed;
        venusOrbitAngle += venusOrbitSpeed; venusSelfAngle += venusSelfSpeed;
        earthOrbitAngle += earthOrbitSpeed; earthSelfAngle += earthSelfSpeed;
        marsOrbitAngle += marsOrbitSpeed; marsSelfAngle += marsSelfSpeed;
        jupiterOrbitAngle += jupiterOrbitSpeed; jupiterSelfAngle += jupiterSelfSpeed;
        saturnOrbitAngle += saturnOrbitSpeed; saturnSelfAngle += saturnSelfSpeed;
        uranusOrbitAngle += uranusOrbitSpeed; uranusSelfAngle += uranusSelfSpeed;
        neptuneOrbitAngle += neptuneOrbitSpeed; neptuneSelfAngle += neptuneSelfSpeed;

        moonOrbitAngle += moonOrbitSpeed; moonSelfAngle += moonSelfSpeed;
        
        // Define the oscillation amplitude
        float oscillationAmplitude = 0.5f;

        // Update and draw planets with orbit and self-rotation including y-axis oscillation
        glm::mat4 mercuryModel = glm::rotate(glm::mat4(1.0f), glm::radians(mercuryOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        mercuryModel = glm::translate(mercuryModel, glm::vec3(mercuryDistance, oscillationAmplitude * sin(glm::radians(mercuryOrbitAngle)), 0.0f));
        mercuryModel = glm::rotate(mercuryModel, glm::radians(mercurySelfAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        mercury.Draw(mercuryModel);
        planetPositions[0] = glm::vec3(mercuryModel[3]);

        glm::mat4 venusModel = glm::rotate(glm::mat4(1.0f), glm::radians(venusOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        venusModel = glm::translate(venusModel, glm::vec3(venusDistance, oscillationAmplitude * sin(glm::radians(venusOrbitAngle)), 0.0f));
        venusModel = glm::rotate(venusModel, glm::radians(venusSelfAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        venus.Draw(venusModel);
        planetPositions[1] = glm::vec3(venusModel[3]);

        glm::mat4 earthModel = glm::rotate(glm::mat4(1.0f), glm::radians(earthOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        earthModel = glm::translate(earthModel, glm::vec3(earthDistance, oscillationAmplitude * sin(glm::radians(earthOrbitAngle)), 0.0f));
        earthModel = glm::rotate(earthModel, glm::radians(earthSelfAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        earth.Draw(earthModel);
        planetPositions[2] = glm::vec3(earthModel[3]);

        // Update and draw the moon orbiting the Earth
        glm::mat4 moonModel = glm::translate(earthModel, glm::vec3(0.0f, 0.0f, 0.0f));
        moonModel = glm::rotate(moonModel, glm::radians(moonOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        moonModel = glm::translate(moonModel, glm::vec3(moonDistance, 0.0f, 0.0f));
        moonModel = glm::rotate(moonModel, glm::radians(moonSelfAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        moon.Draw(moonModel);

        glm::mat4 marsModel = glm::rotate(glm::mat4(1.0f), glm::radians(marsOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        marsModel = glm::translate(marsModel, glm::vec3(marsDistance, oscillationAmplitude * sin(glm::radians(marsOrbitAngle)), 0.0f));
        marsModel = glm::rotate(marsModel, glm::radians(marsSelfAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        mars.Draw(marsModel);
        planetPositions[3] = glm::vec3(marsModel[3]);

        glm::mat4 jupiterModel = glm::rotate(glm::mat4(1.0f), glm::radians(jupiterOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        jupiterModel = glm::translate(jupiterModel, glm::vec3(jupiterDistance, oscillationAmplitude * sin(glm::radians(jupiterOrbitAngle)), 0.0f));
        jupiterModel = glm::rotate(jupiterModel, glm::radians(jupiterSelfAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        jupiter.Draw(jupiterModel);
        planetPositions[4] = glm::vec3(jupiterModel[3]);

        glm::mat4 saturnModel = glm::rotate(glm::mat4(1.0f), glm::radians(saturnOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        saturnModel = glm::translate(saturnModel, glm::vec3(saturnDistance, oscillationAmplitude * sin(glm::radians(saturnOrbitAngle)), 0.0f));
        saturnModel = glm::rotate(saturnModel, glm::radians(saturnSelfAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        saturn.Draw(saturnModel);
        planetPositions[5] = glm::vec3(saturnModel[3]);

        glm::mat4 uranusModel = glm::rotate(glm::mat4(1.0f), glm::radians(uranusOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        uranusModel = glm::translate(uranusModel, glm::vec3(uranusDistance, oscillationAmplitude * sin(glm::radians(uranusOrbitAngle)), 0.0f));
        uranusModel = glm::rotate(uranusModel, glm::radians(uranusSelfAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        uranus.Draw(uranusModel);
        planetPositions[6] = glm::vec3(uranusModel[3]);

        glm::mat4 neptuneModel = glm::rotate(glm::mat4(1.0f), glm::radians(neptuneOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        neptuneModel = glm::translate(neptuneModel, glm::vec3(neptuneDistance, oscillationAmplitude * sin(glm::radians(neptuneOrbitAngle)), 0.0f));
        neptuneModel = glm::rotate(neptuneModel, glm::radians(neptuneSelfAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        neptune.Draw(neptuneModel);
        planetPositions[7] = glm::vec3(neptuneModel[3]);

        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();

        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    diamondTex.Delete();
    skyboxTex.Delete();
    sunTex.Delete();
    mercuryTex.Delete();
    venusTex.Delete();
    earthTex.Delete();
    marsTex.Delete();
    jupiterTex.Delete();
    saturnTex.Delete();
    uranusTex.Delete();
    neptuneTex.Delete();
    shaderProgram.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}