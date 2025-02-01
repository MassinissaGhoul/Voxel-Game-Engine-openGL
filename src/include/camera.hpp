
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../glad/include/glad/glad.h"
#include "../linking/include/glm/ext/matrix_float4x4.hpp"

#include "../linking/include/glm/glm.hpp"
#include "../linking/include/glm/gtc/matrix_transform.hpp"
#include "../linking/include/glm/gtc/type_ptr.hpp"
#include "chunk.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 1.0f;
const float ZOOM = 45.0f;
const float GRAVITY = -9.81f;
class Camera {
    public:
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;
        glm::vec3 cameraRight;
        glm::vec3 veloVec;
        glm::vec3 gravityVec;
        float yaw;
        float pitch;
        float movementSpeed;
        float mouseSensitivity;
        bool isGrounded;
        Camera(Chunk &chunkRef,
               glm::vec3 position = glm::vec3(1.0f, 1.0f, 1.0f),
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
               float pitch = PITCH);
        void update(float deltaTime);
        void jump();
        void movement();
        glm::mat4 getViewMatrix();
        void keyboardInput(Camera_Movement direction, float deltaTime);
        void mouseInput(float xoffset, float yoffset,
                        GLboolean constraintPitch = true);
        void scrollInput(float soffset);

    private:
        Chunk &chunk;
        void updateCameraVectors();
};

#endif
