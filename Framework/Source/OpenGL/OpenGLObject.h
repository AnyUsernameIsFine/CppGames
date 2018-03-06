#pragma once

#include "System\Globals.h"

namespace Framework
{
	class OpenGLObject abstract
	{
	public:
		GLuint getId() const;

	protected:
		bool hasContext() const;
		bool hasContext(const std::string& message) const;

		GLuint id = 0;
	};
}
