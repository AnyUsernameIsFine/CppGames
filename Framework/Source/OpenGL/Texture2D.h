#pragma once

#include "OpenGLObject.h"

namespace Framework
{
	class Texture2D : public OpenGLObject
	{
	public:
		~Texture2D();
		void createEmpty(int width, int height);
		void createFromFile(const std::string& filename);
		int getWidth() const;
		int getHeight() const;
		void use(int unit = 0) const;
		void update(int x, int y, int width, int height, const Byte pixels[]);
		void resize(int width, int height);

	private:
		void create(int width, int height, GLint internalFormat, GLenum format, const Byte pixels[] = nullptr);

		int width;
		int height;
		GLint internalFormat;
		GLenum format;
	};
}
