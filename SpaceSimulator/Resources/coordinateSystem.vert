#version 330 core

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertNormal;

layout (location = 2) in mat4 instMatrix1;
layout (location = 6) in mat4 instMatrix2;
layout (location = 10) in vec4 instColor;
layout (location = 11) in float scale;

out vec3 fragPosition;
out vec3 fragNormal;
out vec4 fragColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	mat4 viewModel = view * instMatrix1 * instMatrix2;

	gl_Position = viewModel * vec4(vertPosition.x * scale, vertPosition.y * scale, vertPosition.z * scale, 1);

	fragPosition = vec3(gl_Position);
	fragNormal = mat3(transpose(inverse(viewModel))) * vertNormal;
	fragColor = instColor;

	gl_Position = projection * gl_Position;
}
