#include "Octahedron.h"

namespace Framework
{
	const std::vector<Mesh::Vertex>& OctahedronMesh::getVertices() const
	{
		return vertices;
	}

	const std::vector<GLushort>& OctahedronMesh::getIndices() const
	{
		return indices;
	}

	void OctahedronMesh::initialize()
	{
		Utility::StaticConstructor<&OctahedronMesh::initialize>::c;

		std::vector<glm::vec3> positions;

		float scale = sqrtf(3);

		for (int i = -1; i <= +1; i += 2) {
			auto permutations = getEvenPermutationsOfVertex(glm::vec3(i, 0, 0) * scale);
			positions.insert(positions.end(), permutations.begin(), permutations.end());
		}

		std::vector<std::vector<int>> faces = {
			{ 0, 2, 1 },
			{ 0, 4, 2 },
			{ 1, 2, 3 },
			{ 2, 4, 3 },
			{ 1, 3, 5 },
			{ 0, 1, 5 },
			{ 0, 5, 4 },
			{ 3, 4, 5 },
		};

		CreatedMesh mesh = createMesh(positions, faces);

		vertices = mesh.vertices;
		indices = mesh.indices;
	}

	std::vector<Mesh::Vertex> OctahedronMesh::vertices;
	std::vector<GLushort> OctahedronMesh::indices;
}
