#include "Framebuffer.h"

namespace GLEngine
{
	Framebuffer::~Framebuffer()
	{
		if (hasContext()) {
			checkGL(glDeleteFramebuffers(1, &id));
		};
	}

	void Framebuffer::create(int width, int height)
	{
		if (id) {
			error("Frame buffer object has already been created");
			return;
		}

		if (!hasContext("Could not create frame buffer object")) {
			return;
		}

		this->width = width;
		this->height = height;

		checkGL(glGenFramebuffers(1, &id));
		checkGL(glBindFramebuffer(GL_FRAMEBUFFER, id));

		checkGL(glGenTextures(1, &texture));
		checkGL(glBindTexture(GL_TEXTURE_2D, texture));

		checkGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));

		checkGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		checkGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		checkGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		checkGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		checkGL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0));

		if (checkGLValue(glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
			error("Framebuffer is not complete");
		}

		checkGL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void Framebuffer::bind(int textureUnit) const
	{
		glGetIntegerv(GL_VIEWPORT, const_cast<GLint*>(viewport));
		glViewport(0, 0, width, height);

		checkGL(glBindFramebuffer(GL_FRAMEBUFFER, id));
		checkGL(glActiveTexture(GL_TEXTURE0 + textureUnit));
		checkGL(glBindTexture(GL_TEXTURE_2D, texture));
	}

	void Framebuffer::unbind() const
	{
		checkGL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	}
}
