#include "Icosahedron.h"

namespace Framework
{
	IcosahedronMesh::IcosahedronMesh()
	{
		initialize();
	}

	const vector<Mesh::Vertex>& IcosahedronMesh::getVertices() const
	{
		return vertices;
	}

	const vector<GLushort>& IcosahedronMesh::getIndices() const
	{
		return indices;
	}

	void IcosahedronMesh::initialize()
	{
		if (vertices.empty()) {
			vector<glm::vec3> positions;

			float phi = (1 + sqrtf(5)) / 2;

			float a = phi / 3;
			float b = (1 + 2 * phi) / 3;
			float s = 1 / sqrtf(a * a + b * b);

			for (int i = -1; i <= +1; i += 2) {
				for (int j = -1; j <= +1; j += 2) {
					auto permutations = getEvenPermutationsOfVertex(glm::vec3(i, j * phi, 0) * s);
					positions.insert(positions.end(), permutations.begin(), permutations.end());
				}
			}

			vector<vector<int>> faces = {
				{ 0, 1, 6 },
				{ 0, 2, 1 },
				{ 0, 4, 8 },
				{ 0, 6, 4 },
				{ 0, 8, 2 },
				{ 1, 2, 7 },
				{ 1, 5, 6 },
				{ 1, 7, 5 },
				{ 2, 3, 7 },
				{ 2, 8, 3 },
				{ 3, 8, 10 },
				{ 3, 9, 7 },
				{ 3, 10, 9 },
				{ 4, 6, 11 },
				{ 4, 10, 8 },
				{ 4, 11, 10 },
				{ 5, 7, 9 },
				{ 5, 9, 11 },
				{ 5, 11, 6 },
				{ 9, 10, 11 },
			};

			CreatedMesh mesh = createMesh(positions, faces);

			vertices = mesh.vertices;
			indices = mesh.indices;
		}
	}

	vector<Mesh::Vertex> IcosahedronMesh::vertices;
	vector<GLushort> IcosahedronMesh::indices;
}
