#pragma once

#include "OpenGLObject.hpp"

#include <string>

namespace Framework
{
	class Texture2D : public OpenGLObject
	{
	public:
		Texture2D(const std::string& filename);
		~Texture2D();

		void use(int unit = 0) const;
	};
}
