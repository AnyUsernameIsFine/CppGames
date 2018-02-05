#include "GLTexture2D.h"
#include "Error.h"

#include <GL\glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Framework
{
	GLTexture2D::GLTexture2D(const std::string& filename)
	{
		glCheck(glGenTextures(1, &id_));
		glCheck(glBindTexture(GL_TEXTURE_2D, id_));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 3);
		//if (data) {
		glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		glCheck(glGenerateMipmap(GL_TEXTURE_2D));
		//}
		//else {
		//	std::cout << "Failed to load texture" << std::endl;
		//}
		glCheck(glBindTexture(GL_TEXTURE_2D, 0));
		stbi_image_free(data);
	}

	GLTexture2D::~GLTexture2D()
	{
	}

	void GLTexture2D::use(unsigned int unit) const
	{
		glCheck(glActiveTexture(GL_TEXTURE0 + unit));
		glCheck(glBindTexture(GL_TEXTURE_2D, id_));
	}
}