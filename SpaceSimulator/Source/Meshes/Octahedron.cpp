#include "Octahedron.h"

namespace Game
{
	const std::vector<float>& OctahedronMesh::getVertices() const
	{
		return vertices_;
	}

	const std::vector<unsigned int>& OctahedronMesh::getIndices() const
	{
		return indices_;
	}

	const std::vector<float> OctahedronMesh::vertices_ = {
		     0,      0, -1.73f,		-1, -1, -1,
		     0,      0, -1.73f,		 1, -1, -1,
		     0,      0, -1.73f,		-1,  1, -1,
		     0,      0, -1.73f,		 1,  1, -1,
		     0, -1.73f,      0,		-1, -1, -1,
		     0, -1.73f,      0,		 1, -1, -1,
		     0, -1.73f,      0,		-1, -1,  1,
		     0, -1.73f,      0,		 1, -1,  1,
		-1.73f,      0,      0,		-1, -1, -1,
		-1.73f,      0,      0,		-1,  1, -1,
		-1.73f,      0,      0,		-1, -1,  1,
		-1.73f,      0,      0,		-1,  1,  1,
		 1.73f,      0,      0,		 1, -1, -1,
		 1.73f,      0,      0,		 1,  1, -1,
		 1.73f,      0,      0,		 1, -1,  1,
		 1.73f,      0,      0,		 1,  1,  1,
		     0,  1.73f,      0,		-1,  1, -1,
		     0,  1.73f,      0,		 1,  1, -1,
		     0,  1.73f,      0,		-1,  1,  1,
		     0,  1.73f,      0,		 1,  1,  1,
		     0,      0,  1.73f,		-1, -1,  1,
		     0,      0,  1.73f,		 1, -1,  1,
		     0,      0,  1.73f,		-1,  1,  1,
		     0,      0,  1.73f,		 1,  1,  1,
	};

	const std::vector<unsigned int> OctahedronMesh::indices_ = {
		 0,  4,  8,
		 1, 12,  5,
		 2,  9, 16,
		 3, 17, 13,
		 6, 20, 10,
		 7, 14, 21,
		11, 22, 18,
		15, 19, 23,
	};
}
