#pragma once

#include "OpenGLObject.h"

namespace GLEngine
{
	class Framebuffer : OpenGLObject
	{
	public:
		Framebuffer() {}
		void create(int width, int height);
		void bind(int textureUnit = 0) const;
		void unbind() const;
		~Framebuffer();
		Framebuffer(const Framebuffer&) = delete;
		Framebuffer(Framebuffer&& other) = delete;
		Framebuffer &operator=(const Framebuffer&) = delete;
		Framebuffer &operator=(Framebuffer&& other) = delete;

	private:
		int width;
		int height;
		GLuint texture;
		GLint viewport[4];
	};
}
