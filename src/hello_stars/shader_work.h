// shader_work.h
#pragma once
#include <string>
#include <vector>

#include <glad/glad.h>

std::string loadShader(const char* path);
unsigned int compileShader(std::string shaderSource, GLenum shaderType);
unsigned int loadAndCompileShader(const char* path, GLenum shaderType);

struct Vertex {
    float x, y, z;
    float r, g, b;
    float u, v;
};

struct Object {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int texture;
    unsigned int numVertices;
};

unsigned int shaders_init();

std::vector<Vertex> generateStarVertices(int spikes, float innerRadius, float outerRadius, float r, float g, float b);
void addStar(float x, float y, std::vector<Object>& stars, const std::vector<Vertex>& starVertices);