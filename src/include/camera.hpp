
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
#include "world.hpp"
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED_WALK = 4.317f;
const float SPEED_SPRINT = 5.612f; // SPEED_WALK * 1.3f

const float SENSITIVITY = 0.4f;
const float ZOOM = 45.0f;
const float GRAVITY = -28.0f;
class World;
class Camera
{
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
    bool gamemode = 0;
    Camera(
        glm::vec3 position = glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
        float pitch = PITCH);

    bool isBlockSolid(int worldX, int worldY, int worldZ);
    bool isCollision(const glm::vec3 &position);

    void worldToChunk(int worldX, int worldY, int worldZ,
                      int &chunkX, int &chunkY, int &chunkZ,
                      int &localX, int &localY, int &localZ,
                      int CHUNK_SIZE, int CHUNK_HEIGHT);
    void setWorld(World *worldPtr);
    void update(float deltaTime);
    void jump();
    void movement();
    glm::mat4 getViewMatrix();
    void keyboardInput(Camera_Movement direction, float deltaTime);
    void mouseInput(float xoffset, float yoffset,
                    GLboolean constraintPitch = true);
    void scrollInput(float soffset);
    glm::vec3 getPosition() const;
    void placeBlock(int worldX, int worldY, int worldZ, glm::vec3 face);
    void rayCast(int option); // 0 == place block 1 == destroy block

private:
    World *worldPtr;
    //Chunk &chunk;
    void updateCameraVectors();
};

#endif
