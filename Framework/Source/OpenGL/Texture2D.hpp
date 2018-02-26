#pragma once

#include "System\Globals.hpp"
#include "OpenGLObject.hpp"

namespace Framework
{
	class Texture2D : public OpenGLObject
	{
	public:
		Texture2D(const string& filename);

		void use(int unit = 0) const;
	};
}
