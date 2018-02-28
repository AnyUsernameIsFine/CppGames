#version 330 core

layout (location = 0) in vec2 vertPosition;
layout (location = 1) in vec2 vertTexCoords;

out vec2 fragTexCoords;

void main()
{
	gl_Position = vec4(vertPosition, 0, 1);

	fragTexCoords = vertTexCoords;
}
