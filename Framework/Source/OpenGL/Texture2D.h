#pragma once

#include "OpenGLObject.h"

namespace Framework
{
	class Texture2D : public OpenGLObject
	{
	public:
		Texture2D() {}
		Texture2D(const string& filename);
		~Texture2D();
		void create(const string& filename);
		void use(int unit = 0) const;
	};
}
