#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D myTexture;

void main()
{
	vec3 normal = normalize(fragNormal);
	vec3 lightDirection = normalize(-fragPosition);
	vec3 viewDirection = normalize(-fragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);

	float ambient = 0.1;
	float diffuse = 0.9 * max(dot(normal, lightDirection), 0);
	float specular = 0.5 * pow(max(dot(viewDirection, reflectDirection), 0), 32);

	color = (ambient + diffuse + specular) * texture(myTexture, fragTexCoords);
}
