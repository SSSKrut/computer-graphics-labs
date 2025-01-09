// shader_work.cpp:
#include "shader_work.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string loadShader(const char* path) {
    std::string shaderSource;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderSource = shaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cerr << "Failed to read shader file " << path << std::endl;
        return 0;
    }

    return shaderSource;
}

unsigned int compileShader(std::string shaderSource, GLenum shaderType) {
    const char* shaderSourceCStr = shaderSource.c_str();
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSourceCStr, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Failed to compile shader:\n" << shaderSource << std::endl;
        std::cerr << infoLog << std::endl;
        return 0;
    }

    return shader;
}

unsigned int loadAndCompileShader(const char* path, GLenum shaderType) {
    std::string shaderSource = loadShader(path);
    if (shaderSource.empty()) {
        return false;
    }

    unsigned int shader = compileShader(shaderSource, shaderType);
    if (shader == 0) {
        return false;
    }

    return shader;
}

unsigned int shaders_init() {
    unsigned int vertexShader = loadAndCompileShader("resources/stars.vert", GL_VERTEX_SHADER);
    unsigned int fragmentShader = loadAndCompileShader("resources/stars.frag", GL_FRAGMENT_SHADER);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Failed to link shader program:\n" << infoLog << std::endl;
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

std::vector<Vertex> generateStarVertices(int spikes, float innerRadius, float outerRadius, float r,
                                         float g, float b) {
    std::vector<Vertex> vertices;
    float angleStep = M_PI / spikes;
    for (int i = 0; i < 2 * spikes; ++i) {
        float angle = i * angleStep;
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        vertices.push_back(Vertex{radius * (float)cos(angle), radius * (float)sin(angle), 0.0f, r,
                                  g, b, 0.0f, 0.0f});
        if (i % 2 != 0) {
            vertices.push_back(Vertex{0.0f, 0.0f, 0.0f, r, g, b, 0.0f, 0.0f});
        }
    }
    for (const auto& vertex : vertices) {
        std::cout << "Vertex: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << "), Color: ("
                  << vertex.r << ", " << vertex.g << ", " << vertex.b << "), TexCoords: ("
                  << vertex.u << ", " << vertex.v << ")" << std::endl;
    }

    return vertices;
}

void addStar(float x, float y, std::vector<Object>& stars,
             const std::vector<Vertex>& starVertices) {
    Object star;

    glGenVertexArrays(1, &star.VAO);
    glGenBuffers(1, &star.VBO);
    glGenBuffers(1, &star.EBO);

    std::vector<Vertex> offsetVertices = starVertices;
    for (auto& vertex : offsetVertices) {
        vertex.x += x;
        vertex.y += y;
    }

    glBindVertexArray(star.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, star.VBO);
    glBufferData(GL_ARRAY_BUFFER, offsetVertices.size() * sizeof(Vertex), offsetVertices.data(),
                 GL_STATIC_DRAW);

    std::vector<unsigned int> indices;
    int numPoints = offsetVertices.size();

    for (int i = 0; i < numPoints; i += 1) {
        indices.push_back(i);
        indices.push_back((i + 1) % numPoints);
        indices.push_back((i + 2) % numPoints);

        indices.push_back((i + 2) % numPoints);
        indices.push_back((i + 1) % numPoints);
        indices.push_back((i + 3) % numPoints);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    star.numVertices = indices.size();
    stars.push_back(star);
}