#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 finalColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mix;
uniform vec3 lightColor;
uniform vec3 lightPosition;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 normal = normalize(fragNormal);
	vec3 lightDirection = normalize(lightPosition - fragPosition);

	float diffuseStrength = 0.9 * max(dot(normal, lightDirection), 0);
	vec3 diffuse = diffuseStrength * lightColor;

	vec3 viewDirection = normalize(-fragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);

	float specularStrength = 0.5 * pow(max(dot(viewDirection, reflectDirection), 0), 32);
	vec3 specular = specularStrength * lightColor;

	vec4 lighting = vec4(ambient + diffuse + specular, 1);

	finalColor = lighting * mix(texture(texture1, fragTexCoords), texture(texture2, fragTexCoords), mix);
}
