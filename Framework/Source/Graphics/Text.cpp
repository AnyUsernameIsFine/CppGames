#include "Text.hpp"
#include "System\Error.hpp"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace Framework
{
	void Text::loadFont(const std::string& filename)
	{
		auto font = std::make_shared<Font>(filename);

		if (!findFont_(font->getFamilyName())) {
			fonts_.push_back(font);
		}
	}

	void Text::setFontFamily(const std::string& family)
	{
		Font* font = findFont_(family.c_str());

		if (font) {
			if (font != font_) {
				font_ = font;
				font_->setSize(size_);
			}
		}
		else {
			error("Font family not loaded");
		}
	}

	void Text::setFontSize(int size)
	{
		if (font_) {
			if (size != size_) {
				size_ = size;
				font_->setSize(size);
			}
		}
		else {
			error("No font set");
		}
	}

	void Text::setFont(const std::string& family, int size)
	{
		setFontFamily(family);
		setFontSize(size);
	}

	void Text::setColor(float r, float g, float b, float a)
	{
		program_->use();
		program_->setUniform("color", glm::vec4(r, g, b, a));
	}

	void Text::draw(float x, float y, const std::string& text) const
	{
		if (font_) {
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

			FT_Pos lineHeight = font_->getLineHeight();

			float lineX = x;
			float lineY = y + lineHeight;

			for (auto c : text) {
				if (c != '\n') {
					const Glyph* glyph = font_->getGlyph(c);

					float xpos = lineX + glyph->left;
					float ypos = glyph->top - lineY - glyph->height;

					float vertices[] = {
						xpos, ypos,									0, 1,
						xpos + glyph->width, ypos,					1, 1,
						xpos, ypos + glyph->height,					0, 0,
						xpos + glyph->width, ypos + glyph->height,	1, 0,
						xpos, ypos + glyph->height,					0, 0,
						xpos + glyph->width, ypos,					1, 1,
					};

					glBindTexture(GL_TEXTURE_2D, glyph->texture);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
					glDrawArrays(GL_TRIANGLES, 0, 6);

					lineX += glyph->advanceX;
				}
				else {
					lineX = x;
					lineY += lineHeight;
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
		else {
			error("No font set");
		}
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

	Font* Text::findFont_(const FT_String* family) const
	{
		bool found = false;
		Font* result = nullptr;

		for (auto i = fonts_.begin(); i != fonts_.end() && !found; i++) {
			if (strcmp(i->get()->getFamilyName(), family) == 0) {
				found = true;
				result = i->get();
			}
		}

		return result;
	}
}
