#include "include/camera.hpp"
#include "linking/include/glm/ext/quaternion_geometric.hpp"
#include "linking/include/glm/ext/vector_float3.hpp"
#include "linking/include/glm/geometric.hpp"
#include <algorithm>
Camera::Camera(Chunk &chunkRef, glm::vec3 position, glm::vec3 up, float yaw,
               float pitch)
    : chunk(chunkRef) {
    this->cameraPos = position;
    this->cameraUp = up;
    this->yaw = YAW;
    this->pitch = PITCH;
    this->mouseSensitivity = SENSITIVITY;
    this->movementSpeed = SPEED;
    this->isGrounded = false;
    this->veloVec = glm::vec3(2.0f, 2.0f, 0.0f);
    this->gravityVec = glm::vec3(0.0f, -2.0f, 0.0f);
    updateCameraVectors();
}

void Camera::update(float deltaTime) {
    /*
        int blockX = static_cast<int>(floor(cameraPos.x));
        int blockY = static_cast<int>(floor(cameraPos.y));
        int blockZ = static_cast<int>(floor(cameraPos.z));
        if (chunk.blocks[blockX][blockY - 1][blockZ] == STONE) {
            this->isGrounded = true;
        }
        if (blockY > 0 && chunk.blocks[blockX][blockY - 1][blockZ] == STONE) {
            this->isGrounded = true;
            this->cameraPos.y = blockY;
            this->veloVec.y = 0.0f;

        } else {
            this->isGrounded = false;
            this->cameraPos = this->cameraPos + this->veloVec * deltaTime;
            this->veloVec = this->veloVec + this->gravityVec * deltaTime;
        }
    */

    // rayCast();
}
void Camera::jump() {
    if (this->isGrounded) {

        this->veloVec.y = 10.0f;
        this->isGrounded = false;
        std::cout << "sauteur\n";
    }
}
void Camera::keyboardInput(Camera_Movement direction, float deltaTime) {
    float velocity = this->movementSpeed * deltaTime;
    /*
    std::cout << "x = " << this->cameraPos.x << "\n"
              << "y  = " << this->cameraPos.y << "\n"
              << "z = " << this->cameraPos.z << "\n";
*/
    int blockX = static_cast<int>(floor(cameraPos.x));
    int blockY = static_cast<int>(floor(cameraPos.y));
    int blockZ = static_cast<int>(floor(cameraPos.z));
    int r = static_cast<int>(cameraFront.x);
    // std::cout << blockX << blockY << blockZ << std::endl;
    switch (direction) {
    case FORWARD:
        /*
    std::cout << "Mouvement détecté: AVANCE " << direction << std::endl;

        if (chunk.blocks[blockX + static_cast<int>(r * velocity)][blockY]
                        [blockZ] != STONE) {
            std::cout << "stone \n";

            // this->cameraPos += cameraFront * velocity;
        }
*/
        this->cameraPos += cameraFront * velocity;
        break;
    case BACKWARD:
        // std::cout << "Mouvement détecté: RECULE " << direction << std::endl;
        this->cameraPos -= cameraFront * velocity;
        break;
    case LEFT:
        // std::cout << "Mouvement détecté: LEFT BOUFFON " << direction <<
        // std::endl;

        this->cameraPos -= cameraRight * velocity;
        break;
    case RIGHT:
        // std::cout << "Mouvement détecté:RIGHT  " << direction << std::endl;

        this->cameraPos += cameraRight * velocity;
        break;
    }
}

void Camera::mouseInput(float xoffset, float yoffset,
                        GLboolean constraintPitch) {
    /*
    std::cout << "Offsets souris: X=" << xoffset << ", Y=" << yoffset
              << std::endl; */
    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;
    this->yaw += xoffset;
    this->pitch += yoffset;
    if (constraintPitch) {
        if (this->pitch > 89.0f) {
            this->pitch = 89.0f;
        }

        if (this->pitch < -89.0f) {
            this->pitch = -89.0f;
        }
    }
    /*
    std::cout << "Yaw: " << this->yaw << ", Pitch: " << this->pitch
              << std::endl; */
    updateCameraVectors();
}

void Camera::scrollInput(float soffset) { return; }

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront,
                       this->cameraUp);
}

glm::vec3 Camera::getPosition() const { return this->cameraPos; }

void Camera::updateCameraVectors() {
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

void Camera::placeBlock(int blockX, int blockY, int blockZ, glm::vec3 face) {
    float absX = abs(face.x);
    float absY = abs(face.y);
    float absZ = abs(face.z);

    if (absX > absY && absX > absZ) {
        if (face.x > 0) {
            chunk.action(blockX + 1, blockY, blockZ, 0);
        } else {
            chunk.action(blockX - 1, blockY, blockZ, 0);
        }
    } else if (absY > absX && absY > absZ) {
        if (face.y > 0) {
            chunk.action(blockX, blockY + 1, blockZ, 0);
        } else {
            chunk.action(blockX, blockY - 1, blockZ, 0);
        }
    } else if (absZ > absX && absZ > absY) {
        if (face.z > 0) {
            chunk.action(blockX, blockY, blockZ + 1, 0);
        } else {

            chunk.action(blockX, blockY, blockZ - 1, 0);
        }
    } else {
        std::cout << "No clear dominant direction!" << std::endl;
    }
}
void Camera::rayCast(int option) {
    glm::vec3 ray;
    const float maxDistance = 6.0f;
    const float stepDistance = 0.1f;
    for (float distance = 0; distance < maxDistance; distance += stepDistance) {
        ray = this->cameraPos + (this->cameraFront * distance);
        int blockX = static_cast<int>(floor(ray.x));
        int blockY = static_cast<int>(floor(ray.y));
        int blockZ = static_cast<int>(floor(ray.z));

        if (blockX >= 0 && blockX < chunk.CHUNK_SIZE && blockY >= 0 &&
            blockY < chunk.CHUNK_SIZE && blockZ >= 0 &&
            blockZ < chunk.CHUNK_SIZE) {

            if (chunk.blocks[blockX][blockY][blockZ] != AIR) {
                if (option == 0) {
                    glm::vec3 face = (ray - glm::vec3(blockX, blockY, blockZ)) -
                                     glm::vec3(0.5f);
                    placeBlock(blockX, blockY, blockZ, face);
                } else {
                    chunk.action(blockX, blockY, blockZ, option);
                }
                return;
            }
        }
    }
}
