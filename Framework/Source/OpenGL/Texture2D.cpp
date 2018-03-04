#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>

namespace Framework
{
	Texture2D::~Texture2D()
	{
		if (hasContext()) {
			checkGL(glDeleteTextures(1, &id));
		}
	}

	void Texture2D::createEmpty(int width, int height)
	{
		create(width, height, GL_RED, GL_RED);

		checkGL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture2D::createFromFile(const string& filename)
	{
		int width, height;
		stbi_set_flip_vertically_on_load(true);
		byte* data = stbi_load(filename.c_str(), &width, &height, nullptr, 3);

		create(width, height, GL_RGB, GL_RGB, data);

		stbi_image_free(data);

		checkGL(glGenerateMipmap(GL_TEXTURE_2D));
		checkGL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	int Texture2D::getWidth() const
	{
		return width;
	}

	int Texture2D::getHeight() const
	{
		return height;
	}

	void Texture2D::use(int unit) const
	{
		checkGL(glActiveTexture(GL_TEXTURE0 + unit));
		checkGL(glBindTexture(GL_TEXTURE_2D, id));
	}

	void Texture2D::update(int x, int y, int width, int height, const byte pixels[])
	{
		GLint unpackAlignment;
		switch (format) {
		case GL_RED:
			unpackAlignment = 1;
			break;

		case GL_RGB:
			unpackAlignment = 3;
			break;
		}

		checkGL(glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment));
		checkGL(glTextureSubImage2D(id, 0, x, y, width, height, format, GL_UNSIGNED_BYTE, pixels));
	}

	void Texture2D::resize(int width, int height)
	{
		int pixelSize;

		switch (format) {
		case GL_RED:
			pixelSize = 1;
			break;

		case GL_RGB:
			pixelSize = 3;
			break;
		}

		GLubyte* data = new GLubyte[this->width * this->height * pixelSize];

		if (width < this->width) {
			glPixelStorei(GL_UNPACK_ROW_LENGTH, this->width);
		}

		checkGL(glBindTexture(GL_TEXTURE_2D, id));
		checkGL(glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, data));
		checkGL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr));
		checkGL(glTextureSubImage2D(id, 0, 0, 0, std::min(width, this->width), std::min(height, this->height), format, GL_UNSIGNED_BYTE, data));
		checkGL(glBindTexture(GL_TEXTURE_2D, 0));

		if (width < this->width) {
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		}

		delete[] data;

		this->width = width;
		this->height = height;
	}

	void Texture2D::create(int width, int height, GLint internalFormat, GLenum format, const byte pixels[])
	{
		if (id) {
			error("2D texture has already been created");
			return;
		}

		if (!hasContext("Could not create 2D texture")) {
			return;
		}

		this->width = width;
		this->height = height;
		this->internalFormat = internalFormat;
		this->format = format;

		checkGL(glGenTextures(1, &id));
		checkGL(glBindTexture(GL_TEXTURE_2D, id));

		checkGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		checkGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		checkGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		checkGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		checkGL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels));
	}
}