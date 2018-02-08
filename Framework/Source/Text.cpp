#include "Text.hpp"
#include "Error.hpp"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace Framework
{
	void Text::setColor(float r, float g, float b, float a)
	{
		program_->use();
		program_->setUniform("color", glm::vec4(r, g, b, a));
	}

	void Text::setFont(const std::string& filename, int size)
	{
		std::string identifier = filename + "@" + std::to_string(size);
		auto result = fonts_.find(identifier);
		if (result == fonts_.end()) {
			font_ = new Font(filename, size);
			fonts_.insert(std::pair<std::string, Font>(identifier, *font_));
		}
		else {
			font_ = &result->second;
		}
	}

	void Text::draw(float x, float y, const std::string& text)
	{
		draw(x, y, std::wstring(text.begin(), text.end()));
	}

	void Text::draw(float x, float y, const std::wstring& text)
	{
		GLboolean depthTest;
		glGetBooleanv(GL_DEPTH_TEST, &depthTest);
		glDisable(GL_DEPTH_TEST);

		GLboolean depthMask;
		glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
		glDepthMask(false);

		GLboolean blend;
		glGetBooleanv(GL_BLEND, &blend);
		glEnable(GL_BLEND);

		GLint blendSrcAlpha, blendDstAlpha;
		glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrcAlpha);
		glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDstAlpha);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		program_->use();
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(vao_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_->getId());

		float lineX = x;
		float lineY = y + font_->height_;

		for (auto c = text.begin(); c != text.end(); c++) {
			if (*c != '\n') {
				Font::Glyph_ glyph = font_->glyphs_[*c];

				float xpos = lineX + glyph.left;
				float ypos = glyph.top - lineY - glyph.height;

				float vertices[] = {
					xpos, ypos,									0.0f, 1.0f,
					xpos + glyph.width, ypos,					1.0f, 1.0f,
					xpos, ypos + glyph.height,					0.0f, 0.0f,
					xpos + glyph.width, ypos + glyph.height,	1.0f, 0.0f,
					xpos, ypos + glyph.height,					0.0f, 0.0f,
					xpos + glyph.width, ypos,					1.0f, 1.0f,
				};

				glBindTexture(GL_TEXTURE_2D, glyph.textureId);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				lineX += glyph.advanceX;
			}
			else {
				lineX = x;
				lineY += font_->height_;
			}
		}

		if (depthTest == GL_TRUE) {
			glEnable(GL_DEPTH_TEST);
		}

		glDepthMask(depthMask);

		if (blend == GL_FALSE) {
			glDisable(GL_BLEND);
		}

		glBlendFunc(blendSrcAlpha, blendDstAlpha);
	}

	void Text::initialize_(int windowWidth, int windowHeight)
	{
		const char* vertexShader =
			"#version 330 core\n"

			"layout(location = 0) in vec4 attributes;"

			"out vec2 vertTexCoords;"

			"uniform mat4 projection;"

			"void main()"
			"{"
			"	gl_Position = projection * vec4(attributes.xy, 0, 1);"
			"	vertTexCoords = attributes.zw;"
			"}";

		const char* fragmentShader =
			"#version 330 core\n"

			"in vec2 vertTexCoords;"

			"out vec4 fragColor;"

			"uniform sampler2D texture;"
			"uniform vec4 color;"

			"void main()"
			"{"
			"	fragColor = color * vec4(1, 1, 1, texture(texture, vertTexCoords).r);"
			"}";

		program_ = new ShaderProgram(vertexShader, fragmentShader, false);

		glm::mat4 projection = glm::ortho(0.0f, (float)windowWidth, -(float)windowHeight, 0.0f);

		program_->use();
		program_->setUniform("projection", projection);

		setColor(0.5f, 0.5f, 0.5f);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);
		vbo_ = new VertexBufferObject({ 4 }, 6);
		glBindVertexArray(0);
	}
}