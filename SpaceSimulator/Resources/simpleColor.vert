#version 330 core

layout (location = 0) in vec3 vertPosition;

uniform mat4 matrix;

void main()
{
	gl_Position = matrix * vec4(vertPosition, 1.0f);
}
