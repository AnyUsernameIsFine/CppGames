#pragma once

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

#include "System\Globals.h"

namespace GLEngine
{
	class Mesh abstract
	{
	public:
		union Vertex
		{
			GLfloat data[8];
			struct
			{
				glm::vec3 position;
				glm::vec3 normal;
				glm::vec2 texCoords;
			};
		};

		virtual const std::vector<Vertex>& getVertices() const = 0;
		virtual const std::vector<GLushort>& getIndices() const = 0;
	};
}
