#include "Tetrahedron.h"

namespace Framework
{
	const vector<Mesh::Vertex>& TetrahedronMesh::getVertices() const
	{
		return vertices;
	}

	const vector<GLushort>& TetrahedronMesh::getIndices() const
	{
		return indices;
	}

	void TetrahedronMesh::initialize()
	{
		Utility::StaticConstructor<&TetrahedronMesh::initialize>::c;

		vector<glm::vec3> positions;

		float sqrt2 = sqrtf(2);

		float scale = sqrtf(3);

		for (int i = -1; i <= +1; i += 2) {
			positions.emplace_back(glm::vec3(i * sqrt2, 0, -1) * scale);
			positions.emplace_back(glm::vec3(0, i * sqrt2, 1) * scale);
		}

		vector<vector<int>> faces = {
			{ 0, 1, 3 },
			{ 0, 2, 1 },
			{ 0, 3, 2 },
			{ 1, 2, 3 },
		};

		CreatedMesh mesh = createMesh(positions, faces);

		vertices = mesh.vertices;
		indices = mesh.indices;
	}

	vector<Mesh::Vertex> TetrahedronMesh::vertices;
	vector<GLushort> TetrahedronMesh::indices;
}
