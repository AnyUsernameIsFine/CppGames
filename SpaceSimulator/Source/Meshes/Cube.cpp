#include "Cube.h"

namespace Game
{
	const std::vector<float>& CubeMesh::getVertices() const
	{
		return vertices_;
	}

	const std::vector<unsigned int>& CubeMesh::getIndices() const
	{
		return indices_;
	}

	const std::vector<float> CubeMesh::vertices_ = {
		-1, -1, -1,		 0,  0, -1,
		-1, -1, -1,		 0, -1,  0,
		-1, -1, -1,		-1,  0,  0,
		 1, -1, -1,		 0,  0, -1,
		 1, -1, -1,		 0, -1,  0,
		 1, -1, -1,		 1,  0,  0,
		-1,  1, -1,		 0,  0, -1,
		-1,  1, -1,		 0,  1,  0,
		-1,  1, -1,		-1,  0,  0,
		 1,  1, -1,		 0,  0, -1,
		 1,  1, -1,		 0,  1,  0,
		 1,  1, -1,		 1,  0,  0,
		-1, -1,  1,		 0,  0,  1,
		-1, -1,  1,		 0, -1,  0,
		-1, -1,  1,		-1,  0,  0,
		 1, -1,  1,		 0,  0,  1,
		 1, -1,  1,		 0, -1,  0,
		 1, -1,  1,		 1,  0,  0,
		-1,  1,  1,		 0,  0,  1,
		-1,  1,  1,		 0,  1,  0,
		-1,  1,  1,		-1,  0,  0,
		 1,  1,  1,		 0,  0,  1,
		 1,  1,  1,		 0,  1,  0,
		 1,  1,  1,		 1,  0,  0,
	};

	const std::vector<unsigned int> CubeMesh::indices_ = {
		0,  6,  3,		 3,  6,  9,
		1,  4, 13,		13,  4, 16,
		2, 14,  8,		 8, 14, 20,
		5, 11, 17,		17, 11, 23,
		7, 19, 10,		10, 19, 22,
		12, 15, 18,		18, 15, 21,
	};
}
