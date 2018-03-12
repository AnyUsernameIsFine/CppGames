#version 430 core

in vec3 fragPosition;
in vec3 fragColor;

out vec4 color;

void main()
{
	float distanceFromCenter = dot(fragPosition.xy, fragPosition.xy) - 1;

	if (distanceFromCenter >= 0) {
		discard;
	}

	color = vec4(fragColor, clamp(fragPosition.z, 0, 1)) * distanceFromCenter * distanceFromCenter;
}
