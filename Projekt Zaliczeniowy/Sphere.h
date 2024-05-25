#ifndef SPHERE_H
#define SPHERE_H
#include <cmath>
#include <glad/glad.h>
#include <vector>
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Sphere {
public:
    Shader& shaderProgram;  
    Texture& texture;
    VAO vao; 
    VBO vbo;
    EBO ebo;
    std::vector<float> sphere_vertices;
    std::vector<unsigned int> sphere_indices;

    Sphere(Shader& shader, Texture& tex, float radius, int stackCount, int sectorCount);
    ~Sphere();

    void Draw(glm::mat4);
    void Draw(glm::mat4,bool);

private:
    void calculateSphere(float radius, int stackCount, int sectorCount);
};

#endif // SPHERE_H
