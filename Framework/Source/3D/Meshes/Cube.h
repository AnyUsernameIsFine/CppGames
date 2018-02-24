#pragma once

#include "PlatonicSolidMesh.h"

namespace Framework
{
	class CubeMesh : public PlatonicSolidMesh
	{
	public:
		CubeMesh();
		const std::vector<Vertex>& getVertices() const;
		const std::vector<GLushort>& getIndices() const;

	private:
		static std::vector<Vertex> vertices_;
		static std::vector<GLushort> indices_;

		static void initialize_();
	};
}
