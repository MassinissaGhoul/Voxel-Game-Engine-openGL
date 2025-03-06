#include "include/camera.hpp"
#include "linking/include/glm/ext/quaternion_geometric.hpp"
#include "linking/include/glm/ext/vector_float3.hpp"
#include "linking/include/glm/geometric.hpp"
#include <algorithm>
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw,
               float pitch)

{
    this->cameraPos = position;
    this->cameraUp = up;
    this->yaw = YAW;
    this->pitch = PITCH;
    this->mouseSensitivity = SENSITIVITY;
    this->movementSpeed = SPEED_WALK;
    this->isGrounded = false;
    this->veloVec = glm::vec3(0.0f, 0.0f, 0.0f);
    this->gravityVec = glm::vec3(0.0f, -2.0f, 0.0f);
    updateCameraVectors();
}

void Camera::setWorld(World *worldPtr)
{
    this->worldPtr = worldPtr;
}

void Camera::update(float deltaTime)
{
    if (this->gamemode)
    {
        int worldX = static_cast<int>(floor(cameraPos.x));
        int worldY = static_cast<int>(floor(cameraPos.y));
        int worldZ = static_cast<int>(floor(cameraPos.z));

        int chunkX, chunkY, chunkZ, localX, localY, localZ;
        worldToChunk(worldX, worldY, worldZ,
                     chunkX, chunkY, chunkZ,
                     localX, localY, localZ, 32, 256);
        size_t key;
        key = this->worldPtr->hashCord(chunkX, chunkZ);

        if (localY > 0 && this->worldPtr->worldMap[key]->blocks[localX][localY - 2][localZ] != AIR)
        {
            this->isGrounded = true;
            this->cameraPos.y = static_cast<float>(localY + 1);
            this->veloVec.y = 0.0f;
        }
        else
        {
            this->isGrounded = false;
            this->veloVec.y += GRAVITY * deltaTime;
            this->cameraPos += this->veloVec * deltaTime;
        }
    }
}

void Camera::jump()
{
    if (this->isGrounded)
    {

        this->veloVec.y = 10.0f;
        this->isGrounded = false;
        std::cout << "sauteur\n";
    }
}
void Camera::keyboardInput(Camera_Movement direction, float deltaTime)
{
    float velocity = SPEED_WALK * deltaTime;
    glm::vec3 front;
    int worldX = static_cast<int>(floor(cameraPos.x));
    int worldY = static_cast<int>(floor(cameraPos.y));
    int worldZ = static_cast<int>(floor(cameraPos.z));

    int chunkX, chunkY, chunkZ, localX, localY, localZ;
    worldToChunk(worldX, worldY, worldZ,
                 chunkX, chunkY, chunkZ,
                 localX, localY, localZ, 32, 256);
    size_t key;
    key = this->worldPtr->hashCord(chunkX, chunkZ);

    if (this->gamemode)
    {
        front = glm::vec3(cameraFront.x, 0.0f, cameraFront.z);
    }
    else
    {
        front = this->cameraFront;
    }
    switch (direction)
    {
    case FORWARD:
        this->cameraPos += front * velocity;

        break;
    case BACKWARD:
        this->cameraPos -= front * velocity;
        break;
    case LEFT:

        this->cameraPos -= cameraRight * velocity;
        break;
    case RIGHT:

        this->cameraPos += cameraRight * velocity;
        break;
    }
}

void Camera::mouseInput(float xoffset, float yoffset,
                        GLboolean constraintPitch)
{
    /*
    std::cout << "Offsets souris: X=" << xoffset << ", Y=" << yoffset
              << std::endl; */
    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;
    this->yaw += xoffset;
    this->pitch += yoffset;
    if (constraintPitch)
    {
        if (this->pitch > 89.0f)
        {
            this->pitch = 89.0f;
        }

        if (this->pitch < -89.0f)
        {
            this->pitch = -89.0f;
        }
    }
    /*
    std::cout << "Yaw: " << this->yaw << ", Pitch: " << this->pitch
              << std::endl; */
    updateCameraVectors();
}

void Camera::scrollInput(float soffset) { return; }

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront,
                       this->cameraUp);
}

glm::vec3 Camera::getPosition() const { return this->cameraPos; }

void Camera::updateCameraVectors()
{
    // Calcul de cameraFront à partir de Yaw et Pitch
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    this->cameraFront = glm::normalize(front);

    // Recalcul de cameraRight (vecteur latéral)
    this->cameraRight = glm::normalize(
        glm::cross(this->cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));

    // Recalcul de cameraUp (vecteur haut)
    this->cameraUp =
        glm::normalize(glm::cross(this->cameraRight, this->cameraFront));
    /*
    std::cout << "Vecteurs mis à jour - Front: " << cameraFront.x << ", "
              << cameraFront.y << ", " << cameraFront.z << std::endl; */
}

void Camera::worldToChunk(int worldX, int worldY, int worldZ,
                          int &chunkX, int &chunkY, int &chunkZ,
                          int &localX, int &localY, int &localZ,
                          int CHUNK_SIZE, int CHUNK_HEIGHT)
{
    chunkX = static_cast<int>(std::floor(static_cast<float>(worldX) / CHUNK_SIZE));
    chunkY = static_cast<int>(std::floor(static_cast<float>(worldY) / CHUNK_HEIGHT));
    chunkZ = static_cast<int>(std::floor(static_cast<float>(worldZ) / CHUNK_SIZE));

    // Calcul des coordonnées locales
    localX = worldX - (chunkX * CHUNK_SIZE);
    localY = worldY - (chunkY * CHUNK_HEIGHT);
    localZ = worldZ - (chunkZ * CHUNK_SIZE);
}

void Camera::placeBlock(int worldX, int worldY, int worldZ, glm::vec3 face)
{
    int chunkX, chunkY, chunkZ, localX, localY, localZ;
    // Conversion initiale des coordonnées mondiales en coordonnées de chunk/local
    worldToChunk(worldX, worldY, worldZ,
                 chunkX, chunkY, chunkZ,
                 localX, localY, localZ, 32, 256);
    if (abs(face.x) > abs(face.y) && abs(face.x) > abs(face.z))
    {
        if (face.x > 0)
            worldX++;
        else
            worldX--;
    }
    else if (abs(face.y) > abs(face.x) && abs(face.y) > abs(face.z))
    {
        if (face.y > 0)
            worldY++;
        else
            worldY--;
    }
    else if (abs(face.z) > abs(face.x) && abs(face.z) > abs(face.y))
    {
        if (face.z > 0)
            worldZ++;
        else
            worldZ--;
    }
    else
    {
        std::cout << "No clear dominant direction!" << std::endl;
        return;
    }

    worldToChunk(worldX, worldY, worldZ,
                 chunkX, chunkY, chunkZ,
                 localX, localY, localZ, 32, 256);

    size_t key;
    key = this->worldPtr->hashCord(chunkX, chunkZ);
    if (this->worldPtr->worldMap.find(key) != this->worldPtr->worldMap.end())
    {
        this->worldPtr->worldMap[key]->action(localX, localY, localZ, 0);
    }
    else
    {
    }
}

void Camera::rayCast(int option)
{
    glm::vec3 ray;
    const float maxDistance = 6.0f;
    const float stepDistance = 0.1f;

    for (float distance = 0; distance < maxDistance; distance += stepDistance)
    {
        ray = this->cameraPos + (this->cameraFront * distance);
        int worldX = static_cast<int>(std::floor(ray.x));
        int worldY = static_cast<int>(std::floor(ray.y));
        int worldZ = static_cast<int>(std::floor(ray.z));

        int chunkX, chunkY, chunkZ, localX, localY, localZ;
        worldToChunk(worldX, worldY, worldZ,
                     chunkX, chunkY, chunkZ,
                     localX, localY, localZ, 32, 256);

        if (localX >= 0 && localX < 32 &&
            localY >= 0 && localY < 256 &&
            localZ >= 0 && localZ < 32)
        {
            size_t keyT;
            keyT = this->worldPtr->hashCord(chunkX, chunkZ);

            if (this->worldPtr->worldMap[keyT]->blocks[localX][localY][localZ] != AIR)
            {
                if (option == 0)
                {
                    glm::vec3 face = (ray - glm::vec3(worldX, worldY, worldZ)) -
                                     glm::vec3(0.5f);
                    placeBlock(worldX, worldY, worldZ, face);
                }
                else
                {
                    size_t key;
                    key = this->worldPtr->hashCord(chunkX, chunkZ);

                    this->worldPtr->worldMap[key]->action(localX, localY, localZ, option);
                }
                return;
            }
        }
    }
}