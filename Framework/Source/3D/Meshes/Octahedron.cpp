#include "Octahedron.hpp"

namespace Framework
{
	OctahedronMesh::OctahedronMesh()
	{
		initialize();
	}

	const vector<Mesh::Vertex>& OctahedronMesh::getVertices() const
	{
		return vertices;
	}

	const vector<GLushort>& OctahedronMesh::getIndices() const
	{
		return indices;
	}

	void OctahedronMesh::initialize()
	{
		if (vertices.empty()) {
			vector<glm::vec3> positions;

			float s = sqrtf(3);

			for (int i = -1; i <= +1; i += 2) {
				auto permutations = evenPermutationsOfVertex(glm::vec3(i, 0, 0) * s);
				positions.insert(positions.end(), permutations.begin(), permutations.end());
			}

			vector<vector<int>> faces = {
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
	}

	vector<Mesh::Vertex> OctahedronMesh::vertices;
	vector<GLushort> OctahedronMesh::indices;
}
