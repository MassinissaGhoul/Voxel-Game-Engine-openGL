
#include "glad/include/glad/glad.h"
#include "include/block.hpp"
#include "include/camera.hpp"
#include "include/shader.h"
#include "include/textureAtlas.hpp"
#include "linking/include/glm/ext/matrix_transform.hpp"
#include "linking/include/glm/glm.hpp"
#include "linking/include/glm/gtc/matrix_transform.hpp"
#include "linking/include/glm/gtc/type_ptr.hpp"
#include "linking/include/stb/stb_image.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
void calculateFPS();
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
Camera camera(glm::vec3(0.0f, 8.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        // std::cout << "LINE\n" << std::endl;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        // std::cout << "FILL" << std::endl;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.keyboardInput(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.keyboardInput(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.keyboardInput(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.keyboardInput(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    std::cout << "Width: " << width << ", Height: " << height << std::endl;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset =
        lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    /*
        std::cout << "Offsets souris corrigés: X=" << xoffset << ", Y=" <<
       yoffset
                  << std::endl;*/
    camera.mouseInput(xoffset, yoffset, true);
}

int main() {
    // Initialisation de GLFW
    if (!glfwInit()) {
        std::cerr << "Erreur : Impossible d'initialiser GLFW !" << std::endl;
        return -1;
    }

    // Configuration de GLFW pour OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Création de la fenêtre
    GLFWwindow *window =
        glfwCreateWindow(800, 600, "Fenêtre OpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erreur : Échec de la création de la fenêtre GLFW !"
                  << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //  Initialisation de GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erreur : Impossible de charger GLAD !" << std::endl;
        return -1;
    }

    // Définition du viewport
    glViewport(0, 0, 800, 600);
    glfwSwapInterval(0);
    glEnable(GL_DEPTH_TEST);
    Shader triShader("shader/tri_vert.vs", "shader/tri_frag.fs");
    TextureAtlas atlas("texture/atlas.png", 16);
    Block block(glm::ivec3(0, 0, 0), DIRT, 2, atlas);
    triShader.use();
    triShader.setInt("atlasTexture", 0);
    // Boucle de rendu
    while (!glfwWindowShouldClose(window)) {

        // Gestion des entrées
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        calculateFPS();

        triShader.setMat4("view", camera.getViewMatrix());
        triShader.setMat4("projection",
                          glm::perspective(glm::radians(45.0f), 800.0f / 600.0f,
                                           0.1f, 100.0f));
        // Effacer le buffer couleur
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        atlas.bind();
        block.render(triShader, atlas);
        // Échanger les buffers et traiter les événements
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Nettoyage
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void calculateFPS() {
    static int frameCount = 0;
    static auto lastTime = std::chrono::high_resolution_clock::now();

    frameCount++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    double elapsedTime =
        std::chrono::duration<double, std::milli>(currentTime - lastTime)
            .count();

    if (elapsedTime > 1000.0) { // Une seconde est passée
        std::cout << "FPS: " << frameCount << std::endl;
        frameCount = 0;
        lastTime = currentTime;
    }
}
