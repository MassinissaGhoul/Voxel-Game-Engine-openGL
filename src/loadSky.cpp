
// loadsky.cpp
#include "include/loadSky.hpp"
#include "linking/include/stb/stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
// Déclarations (tu peux aussi faire un .hpp)
extern GLuint compileShader(const char *source, GLenum type);
extern GLuint createSkyShaderProgram();
extern GLuint loadTexture(const std::string &path);
extern GLuint createSkyQuadVAO();

// --------------------------------------------------------
// Shaders en code source
// --------------------------------------------------------
static const char *SKY_VERT_SRC = R"(
#version 330 core
layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inUV;

out vec2 fragUV;

void main()
{
    gl_Position = vec4(inPos, 0.0, 1.0);
    fragUV = inUV;
}
)";

static const char *SKY_FRAG_SRC = R"(
#version 330 core
in vec2 fragUV;
out vec4 FragColor;

uniform sampler2D skyTexture;

void main()
{
    FragColor = texture(skyTexture, fragUV);
}
)";

// --------------------------------------------------------
// 1) compileShader
// --------------------------------------------------------
GLuint compileShader(const char *source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Erreur compilation shader : " << infoLog << std::endl;
        return 0;
    }
    return shader;
}

// --------------------------------------------------------
// 2) createSkyShaderProgram
// --------------------------------------------------------
GLuint createSkyShaderProgram() {
    // Compile vertex + fragment
    GLuint vert = compileShader(SKY_VERT_SRC, GL_VERTEX_SHADER);
    GLuint frag = compileShader(SKY_FRAG_SRC, GL_FRAGMENT_SHADER);

    // Link program
    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Erreur linkage du programme : " << infoLog << std::endl;
        return 0;
    }

    // Libère les shaders
    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}

// --------------------------------------------------------
// 3) loadTexture (pour PNG unique)
// --------------------------------------------------------
GLuint loadTexture(const std::string &path) {
    int width, height, channels;
    unsigned char *data =
        stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "Impossible de charger l'image : " << path << std::endl;
        return 0;
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Envoi des pixels en OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Paramètres de texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}

// --------------------------------------------------------
// 4) createSkyQuadVAO (quad plein écran, 2 triangles)
// --------------------------------------------------------
GLuint createSkyQuadVAO() {
    // 4 sommets : pos (x,y), uv (u,v)
    float skyVertices[] = {
        //   X,    Y,   U,    V
        -1.0f, 1.0f,  0.0f, 1.0f, // top-left
        -1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        1.0f,  1.0f,  1.0f, 1.0f, // top-right
        1.0f,  -1.0f, 1.0f, 0.0f  // bottom-right
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices,
                 GL_STATIC_DRAW);

    // Attribut 0 : position (2 floats)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)0);

    // Attribut 1 : UV (2 floats)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));

    glBindVertexArray(0);

    return VAO;
}
