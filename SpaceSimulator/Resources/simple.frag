#version 330 core

in vec2 vertTexCoords;

out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mix;

void main()
{
    fragColor = mix(texture(texture1, vertTexCoords), texture(texture2, vertTexCoords), mix);
}
