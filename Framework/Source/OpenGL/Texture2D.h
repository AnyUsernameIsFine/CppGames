#pragma once

#include "OpenGLObject.h"

namespace Framework
{
	class Texture2D : public OpenGLObject
	{
	public:
		~Texture2D();
		void createEmpty(int width, int height);
		void createFromFile(const string& filename);
		int getWidth() const;
		int getHeight() const;
		void use(int unit = 0) const;
		void update(int x, int y, int width, int height, const byte pixels[]);
		void resize(int width, int height);

	private:
		int width;
		int height;
		GLint internalFormat;
		GLenum format;

		void create(int width, int height, GLint internalFormat, GLenum format, const byte pixels[] = nullptr);
	};
}
