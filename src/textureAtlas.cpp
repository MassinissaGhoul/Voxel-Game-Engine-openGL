
#include "include/textureAtlas.hpp"

TextureAtlas::TextureAtlas(const std::string &filePath, int tileSize)
    : tileSize(tileSize) {
    load(filePath);
}

void TextureAtlas::load(const std::string &filePath) {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Charger l'image de l'atlas
    int channels;
    unsigned char *data =
        stbi_load(filePath.c_str(), &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "Erreur : Impossible de charger l'atlas de texture !"
                  << std::endl;
        return;
    }

    // Envoyer la texture à OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void TextureAtlas::bind() const { glBindTexture(GL_TEXTURE_2D, ID); }

glm::vec4 TextureAtlas::getUVs(uint8_t textureIndex) const {
    int atlasWidth = width / 48;   // Nombre de colonnes
    int atlasHeight = height / 45; // Nombre de lignes

    int x = textureIndex % atlasWidth;
    int y = textureIndex / atlasWidth;

    float uvWidth = 1.0f / atlasWidth;
    float uvHeight = 1.0f / atlasHeight;

    return glm::vec4(x * uvWidth, y * uvHeight, (x + 1) * uvWidth,
                     (y + 1) * uvHeight);
}
