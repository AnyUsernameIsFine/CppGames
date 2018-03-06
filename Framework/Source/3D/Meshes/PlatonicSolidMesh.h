#pragma once

#include "3D\Mesh.h"

namespace Framework
{
	class PlatonicSolidMesh abstract : public Mesh
	{
	protected:
		struct CreatedMesh
		{
			std::vector<Vertex> vertices;
			std::vector<GLushort> indices;
		};

		static std::vector<glm::vec3> getEvenPermutationsOfVertex(const glm::vec3& v);
		static std::vector<glm::vec3> getOddPermutationsOfVertex(const glm::vec3& v);
		static std::vector<glm::vec3> getEvenAndOddPermutationsOfVertex(const glm::vec3& v);
		static CreatedMesh createMesh(const std::vector<glm::vec3>& positions, const std::vector<std::vector<int>>& faces);
	};
}
