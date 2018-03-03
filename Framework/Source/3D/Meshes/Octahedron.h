#pragma once

#include "PlatonicSolidMesh.h"

namespace Framework
{
	class OctahedronMesh : public PlatonicSolidMesh
	{
	public:
		const vector<Vertex>& getVertices() const;
		const vector<GLushort>& getIndices() const;

	private:
		static vector<Vertex> vertices;
		static vector<GLushort> indices;

		static void initialize();
	};
}
