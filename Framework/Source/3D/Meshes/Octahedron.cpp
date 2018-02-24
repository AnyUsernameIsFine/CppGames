#include "Octahedron.h"

namespace Framework
{
	OctahedronMesh::OctahedronMesh()
	{
		initialize_();
	}

	const std::vector<Mesh::Vertex>& OctahedronMesh::getVertices() const
	{
		return vertices_;
	}

	const std::vector<unsigned short>& OctahedronMesh::getIndices() const
	{
		return indices_;
	}

	void OctahedronMesh::initialize_()
	{
		if (vertices_.empty()) {
			std::vector<glm::vec3> positions;

			float s = sqrtf(3);

			for (int i = -1; i <= +1; i += 2) {
				auto permutations = evenPermutationsOfVertex_(glm::vec3(i, 0, 0) * s);
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

			vertices_ = mesh.vertices;
			indices_ = mesh.indices;
		}
	}

	std::vector<Mesh::Vertex> OctahedronMesh::vertices_;
	std::vector<unsigned short> OctahedronMesh::indices_;
}
