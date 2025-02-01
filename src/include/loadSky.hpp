
// loadSky.hpp
#ifndef LOADSKY_HPP
#define LOADSKY_HPP

#include "../glad/include/glad/glad.h"
#include <string>

GLuint compileShader(const char *source, GLenum type);
GLuint createSkyShaderProgram();
GLuint loadTexture(const std::string &path);
GLuint createSkyQuadVAO();

#endif
