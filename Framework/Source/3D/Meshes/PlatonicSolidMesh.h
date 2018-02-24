#pragma once

#include "3D/Mesh.h"

namespace Framework
{
	class PlatonicSolidMesh : public Mesh
	{
	public:

	protected:
		struct CreatedMesh
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned short> indices;
		};

		static std::vector<glm::vec3> evenPermutationsOfVertex_(glm::vec3 v);
		static std::vector<glm::vec3> oddPermutationsOfVertex_(glm::vec3 v);
		static std::vector<glm::vec3> evenAndOddPermutationsOfVertex_(glm::vec3 v);
		static CreatedMesh createMesh(const std::vector<glm::vec3>& positions, const std::vector<std::vector<int>>& faces);
	};
}
