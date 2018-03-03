#pragma once

#include "System\Globals.h"

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

namespace Framework
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

		virtual const vector<Vertex>& getVertices() const = 0;
		virtual const vector<GLushort>& getIndices() const = 0;
	};
}
