#version 330 core

layout (location = 0) in vec4 attributes;

out vec2 vertTexCoords;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(attributes.xy, 0, 1);
	vertTexCoords = attributes.zw;
}
