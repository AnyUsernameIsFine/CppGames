#version 330 core

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in mat4 instMatrix;
layout (location = 5) in vec4 instColor;

out vec4 fragColor;

void main()
{
	gl_Position = instMatrix * vec4(vertPosition, 1.0f);
	fragColor = instColor;
}
