#pragma once

#include "System\Globals.hpp"

#include <GL\glew.h>

#define GLM_FORCE_INLINE 
#include <glm\vec3.hpp>

namespace Framework
{
	class Mesh abstract
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

		virtual const vector<Vertex>& getVertices() const = 0;
		virtual const vector<GLushort>& getIndices() const = 0;
	};
}
