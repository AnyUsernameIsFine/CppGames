#version 330 core

in vec2 vertTexCoords;

out vec4 fragColor;

uniform sampler2D texture;
uniform vec4 color;

void main()
{    
    fragColor = color * vec4(1, 1, 1, texture(texture, vertTexCoords).r);
}
