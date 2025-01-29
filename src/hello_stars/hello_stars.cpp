// hello_stars.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "shader_work.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const unsigned int STAR_NUMBER = 60;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Stars", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int shaderProgram = shaders_init();

    std::vector<Object> stars;
    // std::vector<Vertex> starVertices = generateStarVertices(5, 0.1f, 0.5f, 0.0f, 0.0f, 1.0f);
    // addStar(0.0f, 0.0f, stars, starVertices);
    // addStar(0.4f, 0.4f, stars, starVertices);
    // addStar(-0.4f, -0.4f, stars, starVertices);

    // addStar(0.0f, 0.0f, stars, generateStarVertices(10, 0.35f, 0.5f, 1.0f, 0.0f, 0.0f));
    // addStar(0.3f, 0.0f, stars, generateStarVertices(3, 0.35f, 0.5f, 0.5f, 0.0f, 0.5f));
    // addStar(-0.5f, 0.3f, stars, generateStarVertices(7, 0.35f, 0.5f, 1.0f, 0.3f, 0.2f));
    // addStar(0.3f, 0.6f, stars, generateStarVertices(120, 0.1f, 0.3f, 1.0f, 0.1f, 0.0f));
    // addStar(-0.6f, -0.3f, stars, generateStarVertices(5, 0.35f, 0.5f, 1.0f, 0.0f, 0.4f));
    // addStar(0.6f, 0.0f, stars, generateStarVertices(10, 0.35f, 0.1f, 0.2f, 0.5f, 0.2f));

    std::srand(std::time(0));

    for (unsigned int i = 0; i < STAR_NUMBER; ++i) {
        float colorR = (std::rand() % 100) / 100.0f;
        float colorG = (std::rand() % 100) / 100.0f;
        float colorB = (std::rand() % 100) / 100.0f;
        float innerRadius = (std::rand() % 100) / 100.0f * 0.03f;
        float outerRadius = innerRadius*1.5 + (std::rand() % 100) / 100.0f * 0.05f;
        std::vector<Vertex> star = generateStarVertices(5, outerRadius, innerRadius, colorR, colorG, colorB);

        float x = std::rand() % 200 / 100.0f - 1.0f;
        float y = std::rand() % 200 / 100.0f - 1.0f;
        addStar(x, y, stars, star);
    }



    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        for (const auto& star : stars) {
            glBindVertexArray(star.VAO);
            glDrawElements(GL_TRIANGLES, star.numVertices, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}
