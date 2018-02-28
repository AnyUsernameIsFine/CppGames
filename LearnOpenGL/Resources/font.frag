#version 330 core

in vec2 fragTexCoords;

out vec4 finalColor;

uniform sampler2D fontTexture;

void main()
{
    finalColor = texture(fontTexture, fragTexCoords);
}
