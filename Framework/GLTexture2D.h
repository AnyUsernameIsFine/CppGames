#pragma once

#include "GLObject.h"

#include <string>

namespace Framework
{
	class GLTexture2D : public GLObject
	{
	public:
		GLTexture2D(const std::string& filename);
		~GLTexture2D();

		void use(unsigned int unit = 0) const;
	};
}
