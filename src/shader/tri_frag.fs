
#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D atlasTexture; // L’atlas de textures

void main() {
    FragColor = texture(atlasTexture, TexCoord);
}

