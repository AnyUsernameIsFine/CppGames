#pragma once

#include "PlatonicSolidMesh.h"

namespace Framework
{
	class CubeMesh : public PlatonicSolidMesh
	{
	public:
		const std::vector<Vertex>& getVertices() const;
		const std::vector<GLushort>& getIndices() const;

	private:
		static void initialize();

		static std::vector<Vertex> vertices;
		static std::vector<GLushort> indices;
	};
}
