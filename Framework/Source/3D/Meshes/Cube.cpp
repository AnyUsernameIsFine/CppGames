#include "Cube.h"

namespace Framework
{
	CubeMesh::CubeMesh()
	{
		initialize_();
	}

	const std::vector<Mesh::Vertex>& CubeMesh::getVertices() const
	{
		return vertices_;
	}

	const std::vector<unsigned short>& CubeMesh::getIndices() const
	{
		return indices_;
	}

	void CubeMesh::initialize_()
	{
		if (vertices_.empty()) {
			std::vector<glm::vec3> positions;

			for (int i = -1; i <= +1; i += 2) {
				for (int j = -1; j <= +1; j += 2) {
					for (int k = -1; k <= +1; k += 2) {
						positions.emplace_back(glm::vec3(i, j, k));
					}
				}
			}

			std::vector<std::vector<int>> faces = {
				{ 0, 1, 3, 2 },
				{ 0, 4, 5, 1 },
				{ 0, 2, 6, 4 },
				{ 1, 5, 7, 3 },
				{ 2, 3, 7, 6 },
				{ 4, 6, 7, 5 },
			};

			CreatedMesh mesh = createMesh(positions, faces);

			vertices_ = mesh.vertices;
			indices_ = mesh.indices;
		}
	}

	std::vector<Mesh::Vertex> CubeMesh::vertices_;
	std::vector<unsigned short> CubeMesh::indices_;
}
