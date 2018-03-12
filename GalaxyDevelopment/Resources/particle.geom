#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 geomColor[];

uniform mat4 projection;

out vec3 fragColor;
out vec3 fragPosition;

mat4 lookAtCamera(vec3 eye)
{
    vec3 f = normalize(-eye);
    vec3 u = vec3(0, 1, 0);
    vec3 s = normalize(cross(f, u));
    u = cross(s, f);

    mat4 matrix;
	matrix[0] = vec4(s.x, u.x, -f.x, 0);
	matrix[1] = vec4(s.y, u.y, -f.y, 0);
	matrix[2] = vec4(s.z, u.z, -f.z, 0);
	matrix[0] = vec4(-dot(s, eye), -dot(u, eye), dot(f, eye), 1);

    return matrix;
}

void main()
{
	float m22 = projection[2][2];
	float m32 = projection[3][2];
	float nearClippingDistance = m32 / (m22 - 1.0);

	for (int i = -1; i <= 1; i += 2) {
		for (int j = -1; j <= 1; j += 2) {
			fragPosition = vec3(j, i, 0);

			gl_Position = vec4(fragPosition * gl_in[0].gl_PointSize, 0);
			if (false) {
				gl_Position *= lookAtCamera(gl_in[0].gl_Position.xyz);
			}
			gl_Position += gl_in[0].gl_Position;

			fragPosition.z = (-gl_Position.z - nearClippingDistance) / gl_in[0].gl_PointSize;

			gl_Position = projection * gl_Position;

			fragColor = geomColor[0];

			EmitVertex();
		}
	}
}
