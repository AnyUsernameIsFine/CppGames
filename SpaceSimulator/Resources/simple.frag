#version 330 core

in vec2 fragTexCoords;

out vec4 finalColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mix;

void main()
{
    finalColor = mix(texture(texture1, fragTexCoords), texture(texture2, fragTexCoords), mix);
}
