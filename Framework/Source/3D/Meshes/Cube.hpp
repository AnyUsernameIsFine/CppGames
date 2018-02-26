#pragma once

#include "PlatonicSolidMesh.hpp"

namespace Framework
{
	class CubeMesh : public PlatonicSolidMesh
	{
	public:
		CubeMesh();
		const vector<Vertex>& getVertices() const;
		const vector<GLushort>& getIndices() const;

	private:
		static vector<Vertex> vertices;
		static vector<GLushort> indices;

		static void initialize();
	};
}
