#version 330 core

in vec2 fragTexCoords;

out vec4 finalColor;

uniform sampler2D fontTexture;

void main()
{
	vec4 a = texture(fontTexture, fragTexCoords);

	finalColor = vec4(a.r, a.r, a.r, 1);
}
