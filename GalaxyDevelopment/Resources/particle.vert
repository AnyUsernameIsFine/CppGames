#version 430 core

layout(location = 0) in vec4 vertPosition;
layout(location = 1) in float vertSize;
layout(location = 2) in vec3 vertColor;

out vec3 geomColor;

uniform mat4 model;
uniform mat4 view;

void main()
{
    gl_Position = view * model * vertPosition;
	gl_PointSize = vertSize;
	geomColor = vertColor;
}
