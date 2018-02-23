#pragma once

#include <Framework.hpp>

namespace Game
{
	class Mesh abstract
	{
	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
		};

		virtual const std::vector<float>& getVertices() const = 0;
		virtual const std::vector<unsigned int>& getIndices() const = 0;
	};
}
