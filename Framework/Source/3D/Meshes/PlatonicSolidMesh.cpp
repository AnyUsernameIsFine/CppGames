#include "PlatonicSolidMesh.h"

#include <glm\glm.hpp>

namespace Framework
{
	std::vector<glm::vec3> PlatonicSolidMesh::getEvenPermutationsOfVertex(const glm::vec3& v)
	{
		return {
			{ v.x, v.y, v.z },
			{ v.z, v.x, v.y },
			{ v.y, v.z, v.x },
		};
	}

	std::vector<glm::vec3> PlatonicSolidMesh::getOddPermutationsOfVertex(const glm::vec3& v)
	{
		return {
			{ v.x, v.z, v.y },
			{ v.y, v.x, v.z },
			{ v.z, v.y, v.x },
		};
	}

	std::vector<glm::vec3> PlatonicSolidMesh::getEvenAndOddPermutationsOfVertex(const glm::vec3& v)
	{
		return {
			{ v.x, v.y, v.z },
			{ v.z, v.x, v.y },
			{ v.y, v.z, v.x },
			{ v.x, v.z, v.y },
			{ v.y, v.x, v.z },
			{ v.z, v.y, v.x },
		};
	}

	PlatonicSolidMesh::CreatedMesh PlatonicSolidMesh::createMesh(const std::vector<glm::vec3>& positions, const std::vector<std::vector<int>>& faces)
	{
		CreatedMesh mesh;

		bool useIndices = false;
		for (int i = 0; !useIndices && i < static_cast<int>(faces.size()); i++) {
			useIndices = faces[i].size() > 3;
		}

		int indexOffset = 0;

		for (int i = 0; i < static_cast<int>(faces.size()); i++) {
			auto face = faces[i];

			glm::vec3 p0 = positions[face[0]];
			glm::vec3 p1 = positions[face[1]];
			glm::vec3 p2 = positions[face[2]];

			glm::vec3 normal = glm::cross(p1 - p0, p2 - p0);

			for (int j = 0; j < static_cast<int>(face.size()); j++) {
				Vertex vertex;
				vertex.position = positions[face[j]];
				vertex.normal = normal;
				vertex.texCoords = { (vertex.position.x + 1) / 2, (vertex.position.y + 1) / 2 };
				mesh.vertices.emplace_back(vertex);
			}

			if (useIndices) {
				for (int j = 0; j < static_cast<int>(face.size()) - 2; j++) {
					mesh.indices.emplace_back(indexOffset);
					mesh.indices.emplace_back(indexOffset + j + 1);
					mesh.indices.emplace_back(indexOffset + j + 2);
				}
			}

			indexOffset += face.size();
		}

		return mesh;
	}
}
