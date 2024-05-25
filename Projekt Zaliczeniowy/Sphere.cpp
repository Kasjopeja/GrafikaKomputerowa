#include "Sphere.h"


    Sphere::Sphere(Shader& shader, Texture& tex, float radius, int stackCount, int sectorCount)
        : shaderProgram(shader), texture(tex)
    {
        calculateSphere(radius, stackCount, sectorCount);

        vao.Bind();
        vbo = VBO(sphere_vertices.data(), sphere_vertices.size() * sizeof(float));
        ebo = EBO(sphere_indices.data(), sphere_indices.size() * sizeof(unsigned int));

    /*    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
        vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));*/
        
        vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);                  // Position
        vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float))); // Normal
        vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float))); // Color
        vao.LinkAttrib(vbo, 3, 2, GL_FLOAT, 11 * sizeof(float), (void*)(9 * sizeof(float))); // Texture coordinates

        vao.Unbind();
        vbo.Unbind();
        ebo.Unbind();
    }

    Sphere::~Sphere() {
        vao.Delete();
        vbo.Delete();
        ebo.Delete();
    }

    void Sphere::Draw(glm::mat4 model) {
        shaderProgram.Activate();
        texture.Bind();
        vao.Bind();

        GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "modelMatrix");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Ustawienia oœwietlenia
        glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

        glDrawElements(GL_TRIANGLES, sphere_indices.size(), GL_UNSIGNED_INT, 0);

        vao.Unbind();
        texture.Unbind();
    }

    void Sphere::calculateSphere(float radius, int stackCount, int sectorCount) {
        float x, y, z, xy;
        float nx, ny, nz, lengthInv = 1.0f / radius;
        float s, t;
        float r = 1.0f, g = 1.0f, b = 1.0f; // Example color (white)

        float sectorStep = 2 * 3.14159265358979323846f / sectorCount;
        float stackStep = 3.14159265358979323846f / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i) {
            stackAngle = 3.14159265358979323846f / 2 - i * stackStep;
            xy = radius * cosf(stackAngle);
            z = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j) {
                sectorAngle = j * sectorStep;

                x = xy * cosf(sectorAngle);
                y = xy * sinf(sectorAngle);

                // Position
                sphere_vertices.push_back(x);
                sphere_vertices.push_back(y);
                sphere_vertices.push_back(z);

                // Normal
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                sphere_vertices.push_back(nx);
                sphere_vertices.push_back(ny);
                sphere_vertices.push_back(nz);

                // Color
                sphere_vertices.push_back(r);
                sphere_vertices.push_back(g);
                sphere_vertices.push_back(b);

                // Texture coordinates
                s = (float)j / sectorCount;
                t = (float)i / stackCount;
                sphere_vertices.push_back(s);
                sphere_vertices.push_back(t);
            }
        }

        int k1, k2;
        for (int i = 0; i < stackCount; ++i) {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) {
                    sphere_indices.push_back(k1);
                    sphere_indices.push_back(k2);
                    sphere_indices.push_back(k1 + 1);
                }

                if (i != (stackCount - 1)) {
                    sphere_indices.push_back(k1 + 1);
                    sphere_indices.push_back(k2);
                    sphere_indices.push_back(k2 + 1);
                }
            }
        }
    }
