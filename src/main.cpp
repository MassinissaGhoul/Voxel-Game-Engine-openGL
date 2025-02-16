#include "glad/include/glad/glad.h"
#include "include/block.hpp"
#include "include/camera.hpp"
#include "include/chunk.hpp"
#include "include/loadSky.hpp"
#include "include/shader.h"
#include "include/textureAtlas.hpp"
#include "linking/include/glm/ext/matrix_transform.hpp"
#include "linking/include/glm/geometric.hpp"
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
// Camera camera(glm::vec3(0.0f, 8.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
int maxVRAM = 0.0f;
int maxGPU = 0.0f;
Camera *camera;
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

void printGPUMemoryUsage() {
    std::ifstream gpuFile("/sys/class/drm/card1/device/gpu_busy_percent");
    std::ifstream vramFile("/sys/class/drm/card1/device/mem_busy_percent");

    int gpuUsage = 0;
    int vramUsage = 0;

    if (gpuFile.is_open()) {
        gpuFile >> gpuUsage;
        gpuFile.close();
    } else {
        std::cerr << "Impossible de lire l'utilisation GPU." << std::endl;
    }

    if (vramFile.is_open()) {
        vramFile >> vramUsage;
        vramFile.close();
    } else {
        std::cerr << "Impossible de lire l'utilisation mémoire." << std::endl;
    }

    std::cout << "Utilisation GPU : " << gpuUsage << "%" << std::endl;
    std::cout << "Utilisation VRAM : " << vramUsage << "%" << std::endl;
    if (gpuUsage > maxGPU) {
        maxGPU = gpuUsage;
    }

    if (vramUsage > maxVRAM) {
        maxVRAM = vramUsage;
    }
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
    // vsync de mmerde
    glfwSwapInterval(0);
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    Shader triShader("shader/tri_vert.vs", "shader/tri_frag.fs");
    TextureAtlas atlas("texture/atlas.png", 48);
    Block block(glm::ivec3(0, 0, 0), DIRT, 2, atlas);
    std::cout << block.VAO << block.VBO;
    if (block.VAO == 0) {
        std::cerr << "Erreur : VAO du Block non généré !" << std::endl;
        return -1;
    }
    triShader.use();
    triShader.setInt("atlasTexture", 0);
    Chunk chunk(atlas);

    camera = new Camera(chunk, glm::vec3(2.0f, 40.0f, 3.0f));

    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 model2 =
        glm::translate(glm::mat4(1.0f), glm::vec3(32.0f, 32.0f, 32.0f));
    /*
    // sky nul
    GLuint skyProgram = createSkyShaderProgram();
    GLuint skyVAO = createSkyQuadVAO();
    GLuint skyTexture = loadTexture("texture/sky.png"); */

    float renderDistance = 900.0f;
    while (!glfwWindowShouldClose(window)) {

        // Gestion des entrées
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera->update(deltaTime);
        processInput(window);
        calculateFPS();
        triShader.setMat4("view", camera->getViewMatrix());
        triShader.setMat4("projection",
                          glm::perspective(glm::radians(45.0f), 800.0f / 600.0f,
                                           0.1f, 100.0f));
        // Effacer le buffer couleur
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // ff
        /*
        //  ==== Dessin du ciel (quad plein ecran) ====
        glDisable(
            GL_DEPTH_TEST); // pour être sûr que le quad soit derrière tout
        glUseProgram(skyProgram);

        // On bind la texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyTexture);

        // On dit au shader que "skyTexture" = unité 0
        GLint loc = glGetUniformLocation(skyProgram, "skyTexture");
        glUniform1i(loc, 0);

        // Dessiner le quad
        glBindVertexArray(skyVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glUseProgram(0);
        glEnable(GL_DEPTH_TEST);
*/
        // fgf

        // printGPUMemoryUsage();
        atlas.bind();
        triShader.use();

        chunk.draw(triShader, model1);
        glm::vec3 cameraPos = camera->getPosition();
        int cameraChunkX = floor(cameraPos.x / 32);
        int cameraChunkZ = floor(cameraPos.z / 32);
        int minChunkX = cameraChunkX - renderDistance;
        int maxChunkX = cameraChunkX + renderDistance;
        int minChunkZ = cameraChunkZ - renderDistance;
        int maxChunkZ = cameraChunkZ + renderDistance;

        for (int x = minChunkX; x < maxChunkX; x++) {
            for (int z = minChunkZ; z < maxChunkZ; z++) {
                glm::vec3 chunkPosition(x * 32, 0.0f, z * 32);
                // std::cout << minChunkX << "puiis " << minChunkZ << std::endl;
                float distance = glm::distance(chunkPosition, cameraPos);
                if (distance < renderDistance) {
                    glm::mat4 model2 =
                        glm::translate(glm::mat4(1.0f), chunkPosition);
                    chunk.draw(triShader, model2);
                }
            }
        }

        block.render(triShader, atlas);

        //  Échanger les buffers et traiter les événements
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    std::cout << "FINAL DATA GPU = maxgpu and max vram" << maxGPU << maxVRAM
              << std::endl;
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

    if (elapsedTime > 1000.0) {
        std::cout << "FPS: " << frameCount << std::endl;
        frameCount = 0;
        lastTime = currentTime;
    }
}
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
        camera->keyboardInput(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->keyboardInput(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->keyboardInput(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->keyboardInput(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera->jump();
    }
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
    camera->mouseInput(xoffset, yoffset, true);
}
/*
void worldRender(glm::vec3 cameraPos, float renderDistance) {
    int cameraChunkX = floor(cameraPos.x / 32);
    int cameraChunkZ = floor(cameraPos.z / 32);
    int minChunkX = cameraChunkX - static_cast<int>(renderDistance);
    int maxChunkX = cameraChunkX + static_cast<int>(renderDistance);
    int minChunkZ = cameraChunkZ - static_cast<int>(renderDistance);
    int maxChunkZ = cameraChunkZ + static_cast<int>(renderDistance);

    for (int x = minChunkX; x < maxChunkX; x++) {
        for (int z = minChunkZ; z < maxChunkZ; z++) {
            glm::vec3 chunkPosition(x * 32, 0.0f, z * 32);

            float distance = glm::distance(chunkPosition, cameraPos);
            if (distance < renderDistance) {
                glm::mat4 model2 =
                    glm::translate(glm::mat4(1.0f), chunkPosition);
                chunk.draw(triShader, model2);
            }
        }
    }
}

*/
