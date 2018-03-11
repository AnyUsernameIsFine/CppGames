#pragma once

#include "OpenGLObject.h"

namespace GLEngine
{
	class Texture2D : public OpenGLObject
	{
	public:
		Texture2D() {}
		~Texture2D();
		void createEmpty(int width, int height);
		void createFromFile(const std::string& filename);
		int getWidth() const;
		int getHeight() const;
		void use(int unit = 0) const;
		void update(int x, int y, int width, int height, const Byte pixels[]);
		void resize(int width, int height);
		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&& other) = delete;
		Texture2D &operator=(const Texture2D&) = delete;
		Texture2D &operator=(Texture2D&& other) = delete;

	private:
		void create(int width, int height, GLint internalFormat, GLenum format, const Byte pixels[] = nullptr);

		int width;
		int height;
		GLint internalFormat;
		GLenum format;
	};
}
