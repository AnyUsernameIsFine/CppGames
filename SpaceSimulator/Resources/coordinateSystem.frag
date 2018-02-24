#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;
in vec4 fragColor;

out vec4 finalColor;

void main()
{
	vec3 normal = normalize(fragNormal);
	vec3 lightDirection = normalize(-fragPosition);

	float ambient = 0.5;
	float diffuse = 0.5 * max(0, dot(normal, lightDirection));

	finalColor = vec4((ambient + diffuse) * fragColor.rgb, fragColor.a);
}
