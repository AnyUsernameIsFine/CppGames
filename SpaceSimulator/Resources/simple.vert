#version 330 core

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec2 vertTexCoords;

out vec2 fragTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vertPosition, 1.0f);
	fragTexCoords = vertTexCoords;
}
