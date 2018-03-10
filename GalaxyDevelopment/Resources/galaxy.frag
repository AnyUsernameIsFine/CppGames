#version 430 core

in vec4 fragPosition;
in vec4 fragColor;

out vec4 color;

void main()
{
	float distanceFromCenter = dot(fragPosition.xy, fragPosition.xy) - 1;

	if (distanceFromCenter >= 0) {
		discard;
	}

	color = fragColor * distanceFromCenter * distanceFromCenter;
	color.a *= clamp(fragPosition.z, 0, 1);
}
