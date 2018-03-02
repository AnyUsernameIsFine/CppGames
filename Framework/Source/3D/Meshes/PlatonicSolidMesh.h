#pragma once

#include "3D/Mesh.h"

namespace Framework
{
	class PlatonicSolidMesh abstract : public Mesh
	{
	protected:
		struct CreatedMesh
		{
			vector<Vertex> vertices;
			vector<GLushort> indices;
		};

		static vector<glm::vec3> getEvenPermutationsOfVertex(const glm::vec3& v);
		static vector<glm::vec3> getOddPermutationsOfVertex(const glm::vec3& v);
		static vector<glm::vec3> getEvenAndOddPermutationsOfVertex(const glm::vec3& v);
		static CreatedMesh createMesh(const vector<glm::vec3>& positions, const vector<vector<int>>& faces);
	};
}
