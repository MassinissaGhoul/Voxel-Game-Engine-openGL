
#include "glad/include/glad/glad.h"
#include "linking/include/glm/ext/matrix_transform.hpp"
#include "linking/include/glm/glm.hpp"
#include "linking/include/glm/gtc/matrix_transform.hpp"
#include "linking/include/glm/gtc/type_ptr.hpp"
#include "linking/include/stb/stb_image.h"
#include "shader.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
// Prototypes des fonctions
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const int gridX = 5;
const int gridY = 5;
const float spacing = 2.2f;
int main() {

    int success;
    char infoLog[512];

    // === Initialisation GLFW ===
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW.\n";
        return -1;
    }

    // Configuration du contexte OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Création de la fenêtre
    GLFWwindow *window =
        glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window.\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialisation de GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD.\n";
        return -1;
    }

    // Configuration de la fenêtre
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
    //=== Triangle avec gradient
    Shader triShader("tri_vert.vs", "tri_frag.fs");
    /*
    float vertices[] = {
        // positions        // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top left
    };
*/

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
    /*

    float vertices[] = {
        // Position           // Texture
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 0: avant bas gauche
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // 1: avant bas droite
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  // 2: avant haut droite
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  // 3: avant haut gauche
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  // 4: arrière bas gauche
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  // 5: arrière bas droite
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  // 6: arrière haut droite
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f   // 7: arrière haut gauche
    };
    */
    unsigned int indices[] = {// Face avant
                              0, 1, 2, 2, 3, 0,
                              // Face arrière
                              4, 5, 6, 6, 7, 4,
                              // Face gauche
                              0, 3, 7, 7, 4, 0,
                              // Face droite
                              1, 2, 6, 6, 5, 1,
                              // Face dessus
                              3, 2, 6, 6, 7, 3,
                              // Face dessous
                              0, 1, 5, 5, 4, 0};
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data =
        stbi_load("texture/classicshit.jpg", &width, &height, &nrChannels, 0);
    unsigned int texture, texture2;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // set texture wrapping to GL_REPEAT
                                // (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //  set texture filtering parameters

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    // texture 2

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("texture/AAA.png", &width, &height, &nrChannels, 0);
    if (data) {
        // note that the awesomeface.png has transparency and thus an alpha
        // channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture\n"
                  << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(data);
    triShader.use();
    triShader.setInt("texture1", 0);
    triShader.setInt("texture2", 1);
    glBindVertexArray(VAO);

    // plei de cube

    // === Boucle principale ===
    while (!glfwWindowShouldClose(window)) {
        // Entrées utilisateur
        processInput(window);

        // Couleur de fond
        glClearColor(0.0016f, 0.0012f, 0.252f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Rendu
        glClear(GL_COLOR_BUFFER_BIT);

        // triShader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        triShader.use();
        /*
                // create transformations
                glm::mat4 model = glm::mat4(
                    1.0f); // make sure to initialize matrix to identity matrix
           first glm::mat4 view = glm::mat4(1.0f); glm::mat4 projection =
           glm::mat4(1.0f); model = glm::rotate(model, (float)glfwGetTime() *
           glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f)); view =
           glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); projection =
           glm::perspective(glm::radians(105.0f), (float)SCR_WIDTH /
           (float)SCR_HEIGHT, 0.1f, 100.0f);

                // retrieve the matrix uniform locations
                // unsigned int modelLoc = glGetUniformLocation(triShader.ID,
           "model");
                // unsigned int viewLoc = glGetUniformLocation(triShader.ID,
           "view");
                // pass them to the shaders (3 different ways)
                // glUniformMatrix4fv(modelLoc, 1, GL_FALSE,
           glm::value_ptr(model));
                // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
                triShader.setMat4("view", view);
                triShader.setMat4("model", model);
                // note: currently we set the projection matrix each frame, but
           since
                // the projection matrix rarely changes it's often best practice
           to set
                // it outside the main loop only once.
                triShader.setMat4("projection", projection);
                */

        for (int x = 0; x < gridX; x++) {
            for (int y = 0; y < gridY; y++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(
                    model, glm::vec3(x * spacing, y * spacing, 0.0f));

                glm::mat4 view = glm::mat4(1.0f);
                glm::mat4 projection = glm::mat4(1.0f);
                model = glm::rotate(model,
                                    (float)glfwGetTime() * glm::radians(50.0f),
                                    glm::vec3(0.5f, 1.0f, 0.0f));
                view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
                projection = glm::perspective(
                    glm::radians(145.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT,
                    0.1f, 100.0f);
                // balance au shader
                triShader.setMat4("model", model);
                triShader.setMat4("view", view);
                // note: currently we set the projection matrix each frame, but
                // since the projection matrix rarely changes it's often best
                // practice to set it outside the main loop only once.
                triShader.setMat4("projection", projection);
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //  Gestion des événements
        glfwPollEvents();
    }

    // Nettoyage
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    return 0;
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
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    std::cout << "Width: " << width << ", Height: " << height << std::endl;
    glViewport(0, 0, width, height);
}
