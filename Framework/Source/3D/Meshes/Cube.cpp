#include "Cube.h"

namespace Framework
{
	const vector<Mesh::Vertex>& CubeMesh::getVertices() const
	{
		return vertices;
	}

	const vector<GLushort>& CubeMesh::getIndices() const
	{
		return indices;
	}

	void CubeMesh::initialize()
	{
		Utility::StaticConstructor<&CubeMesh::initialize>::c;

		vector<glm::vec3> positions;

		for (int i = -1; i <= +1; i += 2) {
			for (int j = -1; j <= +1; j += 2) {
				for (int k = -1; k <= +1; k += 2) {
					positions.emplace_back(glm::vec3(i, j, k));
				}
			}
		}

		vector<vector<int>> faces = {
			{ 0, 1, 3, 2 },
			{ 0, 4, 5, 1 },
			{ 0, 2, 6, 4 },
			{ 1, 5, 7, 3 },
			{ 2, 3, 7, 6 },
			{ 4, 6, 7, 5 },
		};

		CreatedMesh mesh = createMesh(positions, faces);

		vertices = mesh.vertices;
		indices = mesh.indices;
	}

	vector<Mesh::Vertex> CubeMesh::vertices;
	vector<GLushort> CubeMesh::indices;
}
