#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Framework
{
	Texture2D::Texture2D(const string& filename)
	{
		create(filename);
	}

	Texture2D::~Texture2D()
	{
		if (sdlCheckValue(SDL_GL_GetCurrentContext())) {
			console(id);
			glCheck(glDeleteTextures(1, &id));
		}
	}
	
	void Texture2D::create(const string& filename)
	{
		if (!hasContext("Could not create 2D texture")) {
			return;
		}

		if (id) {
			error("2D texture has already been created");
			return;
		}

		glCheck(glGenTextures(1, &id));
		glCheck(glBindTexture(GL_TEXTURE_2D, id));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		byte* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 3);
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

	void Texture2D::use(int unit) const
	{
		glCheck(glActiveTexture(GL_TEXTURE0 + unit));
		glCheck(glBindTexture(GL_TEXTURE_2D, id));
	}
}