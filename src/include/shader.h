
#ifndef SHADER_H
#define SHADER_H

#include "../glad/include/glad/glad.h"
#include "../linking/include/glm/ext/matrix_float4x4.hpp"

#include "../linking/include/glm/glm.hpp"
#include "../linking/include/glm/gtc/matrix_transform.hpp"
#include "../linking/include/glm/gtc/type_ptr.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
    public:
        // the program ID
        unsigned int ID;

        // constructor reads and builds the shader
        Shader(const char *vertexPath, const char *fragmentPath);
        // use/activate the shader
        void use();
        // utility uniform functions
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;

    private:
        void checkCompileErrors(unsigned int shader, std::string type);
};

#endif
