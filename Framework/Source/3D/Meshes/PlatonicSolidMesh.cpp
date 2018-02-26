#include "PlatonicSolidMesh.hpp"

#define GLM_FORCE_INLINE
#include <glm\glm.hpp>

namespace Framework
{
	vector<glm::vec3> PlatonicSolidMesh::evenPermutationsOfVertex(const glm::vec3& v)
	{
		return {
			{ v.x, v.y, v.z },
			{ v.z, v.x, v.y },
			{ v.y, v.z, v.x },
		};
	}

	vector<glm::vec3> PlatonicSolidMesh::oddPermutationsOfVertex(const glm::vec3& v)
	{
		return {
			{ v.x, v.z, v.y },
			{ v.y, v.x, v.z },
			{ v.z, v.y, v.x },
		};
	}

	vector<glm::vec3> PlatonicSolidMesh::evenAndOddPermutationsOfVertex(const glm::vec3& v)
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

	PlatonicSolidMesh::CreatedMesh PlatonicSolidMesh::createMesh(const vector<glm::vec3>& positions, const vector<vector<int>>& faces)
	{
		CreatedMesh mesh;

		bool useIndices = false;
		for (int i = 0; !useIndices && i < (int)faces.size(); i++) {
			useIndices = faces[i].size() > 3;
		}

		int indexOffset = 0;

		for (int i = 0; i < (int)faces.size(); i++) {
			auto face = faces[i];

			glm::vec3 p0 = positions[face[0]];
			glm::vec3 p1 = positions[face[1]];
			glm::vec3 p2 = positions[face[2]];

			glm::vec3 normal = glm::cross(p1 - p0, p2 - p0);

			for (int j = 0; j < (int)face.size(); j++) {
				Vertex vertex;
				vertex.position = positions[face[j]];
				vertex.normal = normal;
				mesh.vertices.emplace_back(vertex);
			}

			if (useIndices) {
				for (int j = 0; j < (int)face.size() - 2; j++) {
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
