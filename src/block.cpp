#include "include/block.hpp"

Block::Block(glm::ivec3 blockPos, blockType type, uint8_t textureIndex,
             TextureAtlas &atlas)
    : blockPos(blockPos),
      type(type),
      textureIndex(textureIndex) {
    setupMesh(atlas);
}

Block::~Block() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Block::setupMesh(TextureAtlas &atlas) {
    glm::vec4 uvs = atlas.getUVs(textureIndex);

    float vertices[] = {
        // Face arrière (Back)
        -0.5f,
        -0.5f,
        -0.5f,
        uvs.x,
        uvs.w,
        0.5f,
        -0.5f,
        -0.5f,
        uvs.z,
        uvs.w,
        0.5f,
        0.5f,
        -0.5f,
        uvs.z,
        uvs.y,
        0.5f,
        0.5f,
        -0.5f,
        uvs.z,
        uvs.y,
        -0.5f,
        0.5f,
        -0.5f,
        uvs.x,
        uvs.y,
        -0.5f,
        -0.5f,
        -0.5f,
        uvs.x,
        uvs.w,

        // Face avant (Front)
        -0.5f,
        -0.5f,
        0.5f,
        uvs.x,
        uvs.w,
        0.5f,
        -0.5f,
        0.5f,
        uvs.z,
        uvs.w,
        0.5f,
        0.5f,
        0.5f,
        uvs.z,
        uvs.y,
        0.5f,
        0.5f,
        0.5f,
        uvs.z,
        uvs.y,
        -0.5f,
        0.5f,
        0.5f,
        uvs.x,
        uvs.y,
        -0.5f,
        -0.5f,
        0.5f,
        uvs.x,
        uvs.w,

        // Face gauche (Left)
        -0.5f,
        0.5f,
        0.5f,
        uvs.x,
        uvs.y,
        -0.5f,
        0.5f,
        -0.5f,
        uvs.z,
        uvs.y,
        -0.5f,
        -0.5f,
        -0.5f,
        uvs.z,
        uvs.w,
        -0.5f,
        -0.5f,
        -0.5f,
        uvs.z,
        uvs.w,
        -0.5f,
        -0.5f,
        0.5f,
        uvs.x,
        uvs.w,
        -0.5f,
        0.5f,
        0.5f,
        uvs.x,
        uvs.y,

        // Face droite (Right)
        0.5f,
        0.5f,
        0.5f,
        uvs.x,
        uvs.y,
        0.5f,
        0.5f,
        -0.5f,
        uvs.z,
        uvs.y,
        0.5f,
        -0.5f,
        -0.5f,
        uvs.z,
        uvs.w,
        0.5f,
        -0.5f,
        -0.5f,
        uvs.z,
        uvs.w,
        0.5f,
        -0.5f,
        0.5f,
        uvs.x,
        uvs.w,
        0.5f,
        0.5f,
        0.5f,
        uvs.x,
        uvs.y,

        // Face en bas (Bottom)
        -0.5f,
        -0.5f,
        -0.5f,
        uvs.x,
        uvs.y,
        0.5f,
        -0.5f,
        -0.5f,
        uvs.z,
        uvs.y,
        0.5f,
        -0.5f,
        0.5f,
        uvs.z,
        uvs.w,
        0.5f,
        -0.5f,
        0.5f,
        uvs.z,
        uvs.w,
        -0.5f,
        -0.5f,
        0.5f,
        uvs.x,
        uvs.w,
        -0.5f,
        -0.5f,
        -0.5f,
        uvs.x,
        uvs.y,

        // Face en haut (Top)
        -0.5f,
        0.5f,
        -0.5f,
        uvs.x,
        uvs.y,
        0.5f,
        0.5f,
        -0.5f,
        uvs.z,
        uvs.y,
        0.5f,
        0.5f,
        0.5f,
        uvs.z,
        uvs.w,
        0.5f,
        0.5f,
        0.5f,
        uvs.z,
        uvs.w,
        -0.5f,
        0.5f,
        0.5f,
        uvs.x,
        uvs.w,
        -0.5f,
        0.5f,
        -0.5f,
        uvs.x,
        uvs.y,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));

    glBindVertexArray(0);
}

void Block::render(Shader &shader, TextureAtlas &atlas) {
    shader.use();
    atlas.bind();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(blockPos));
    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
