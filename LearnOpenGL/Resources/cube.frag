#version 330 core

in vec2 vertTexCoords;

out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mix;
uniform vec4 lightColor;

void main()
{
    fragColor = lightColor * mix(texture(texture1, vertTexCoords), texture(texture2, vertTexCoords), mix);
}
