#include "include/camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    this->cameraPos = position;
    this->cameraUp = up;
    this->yaw = YAW;
    this->pitch = PITCH;
    this->mouseSensitivity = SENSITIVITY;
    this->movementSpeed = SPEED;
    updateCameraVectors();
}

void Camera::keyboardInput(Camera_Movement direction, float deltaTime) {
    float velocity = this->movementSpeed * deltaTime;
    switch (direction) {
    case FORWARD:
        std::cout << "Mouvement détecté: AVANCE " << direction << std::endl;

        this->cameraPos += cameraFront * velocity;
        break;
    case BACKWARD:
        // std::cout << "Mouvement détecté: RECULE " << direction << std::endl;

        this->cameraPos -= cameraFront * velocity;
        break;
    case LEFT:
        // std::cout << "Mouvement détecté: LEFT " << direction << std::endl;

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
