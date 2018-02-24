#pragma once

#include "PlatonicSolidMesh.h"

namespace Framework
{
	class IcosahedronMesh : public PlatonicSolidMesh
	{
	public:
		IcosahedronMesh();
		const std::vector<Vertex>& getVertices() const;
		const std::vector<unsigned short>& getIndices() const;

	private:
		static std::vector<Vertex> vertices_;
		static std::vector<unsigned short> indices_;

		static void initialize_();
	};
}
