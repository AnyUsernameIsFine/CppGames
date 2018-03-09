#version 430 core

layout(location = 0) in vec4 vertPosition;
layout(location = 1) in vec2 vertTexCoords;

out vec2 fragTexCoords;

void main()
{
	gl_Position = vertPosition;

	fragTexCoords = vertTexCoords;
}
