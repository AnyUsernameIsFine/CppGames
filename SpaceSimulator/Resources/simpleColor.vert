#version 330 core

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in mat4 instMatrix1;
layout (location = 5) in mat4 instMatrix2;
layout (location = 9) in vec4 instColor;

out vec4 fragColor;

uniform mat4 projectionViewMatrix;

void main()
{
	gl_Position = projectionViewMatrix * instMatrix1 * instMatrix2 * vec4(vertPosition, 1.0f);
	fragColor = instColor;
}
