#pragma once

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
			float data[6];
			struct
			{
				glm::vec3 position;
				glm::vec3 normal;
			};
		};

		virtual const std::vector<Vertex>& getVertices() const = 0;
		virtual const std::vector<unsigned short>& getIndices() const = 0;
	};
}
