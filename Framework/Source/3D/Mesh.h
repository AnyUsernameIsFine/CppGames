#pragma once

#include <GL\glew.h>

#define GLM_FORCE_INLINE 
#include <glm\vec3.hpp>

#include <vector>

namespace Framework
{
	class Mesh
	{
	public:
		union Vertex
		{
			GLfloat data[6];
			struct
			{
				glm::vec3 position;
				glm::vec3 normal;
			};
		};

		virtual const std::vector<Vertex>& getVertices() const = 0;
		virtual const std::vector<GLushort>& getIndices() const = 0;
	};
}
