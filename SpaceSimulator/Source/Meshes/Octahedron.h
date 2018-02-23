#pragma once

#include "Mesh.h"

namespace Game
{
	class OctahedronMesh : public Mesh
	{
	public:
		const std::vector<float>& getVertices() const;
		const std::vector<unsigned int>& getIndices() const;

	private:
		const static std::vector<float> vertices_;
		const static std::vector<unsigned int> indices_;
	};
}
