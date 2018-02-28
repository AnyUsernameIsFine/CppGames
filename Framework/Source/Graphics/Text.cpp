#include "Text.hpp"
#include "System\Error.hpp"

#define GLM_FORCE_INLINE 
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace Framework
{
	void Text::loadFont(const string& filename, const string& family)
	{
		if (family.empty()) {
			auto font = std::make_shared<Font>();

			if (font->loadFromFile(filename) == 0 && !findFont(font->getFamilyName())) {
				fonts[font->getFamilyName()] = font;
			}
		}
		else if (!findFont(family)) {
			auto font = std::make_shared<Font>();

			if (font->loadFromFile(filename) == 0) {
				fonts[family] = font;
			}
		}
	}

	void Text::setFontFamily(const string& family)
	{
		Font* font = findFont(family);

		if (font) {
			if (font != this->font) {
				this->font = font;
				this->font->setSize(size);
			}
		}
		else {
			error("Font family not loaded");
		}
	}

	void Text::setFontSize(int size)
	{
		if (!font) {
			error("No font set");
		}
		else {
			if (size != this->size) {
				this->size = size;
				font->setSize(size);
			}
		}
	}

	void Text::setFont(const string& family, int size)
	{
		setFontFamily(family);
		setFontSize(size);
	}

	void Text::setColor(float r, float g, float b, float a)
	{
		program->use();
		program->setUniform("color", glm::vec4(r, g, b, a));
	}

	int Text::getFontHeight() const
	{
		return font->getHeight();
	}

	GLuint Text::getFontTextureId() const
	{
		if (!font) {
			return 0;
		}

		return font->getTextureId();
	}

	StringStream Text::operator()(float x, float y)
	{
		DrawConfiguration drawConfiguration = { this, x, y };

		return {
			drawFromStream,
			&drawConfiguration,
			sizeof(DrawConfiguration)
		};
	}

	void Text::initialize(int windowWidth, int windowHeight)
	{
		this->windowWidth = windowWidth;
		this->windowHeight = windowHeight;

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

			"uniform sampler2D fontSizeTexture;"
			"uniform vec4 color;"

			"void main()"
			"{"
			"	fragColor = color * vec4(1, 1, 1, texture(fontSizeTexture, vertTexCoords).r);"
			"}";

		program = new ShaderProgram(vertexShader, fragmentShader, false);

		applyWindowSize();

		setColor(0.5f, 0.5f, 0.5f);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		vertices = new GlyphQuad[MAX_STRING_LENGTH];
		vbo = new VertexBufferObject({ 4 }, 6 * MAX_STRING_LENGTH);
		glBindVertexArray(0);
	}

	void Text::onWindowResize(int width, int height)
	{
		windowHasResized = true;

		windowWidth = width;
		windowHeight = height;
	}

	Font* Text::findFont(const string& family) const
	{
		bool found = false;
		Font* result = nullptr;

		fonts.find(family);

		auto it = fonts.find(family);
		if (it != fonts.end()) {
			found = true;
			result = it->second.get();
		}

		return result;
	}

	void Text::draw(float x, float y, const std::u32string& text)
	{
		if (!font) {
			error("No font set");

			return;
		}

		if (windowHasResized) {
			windowHasResized = false;
			applyWindowSize();
		}

		FT_Pos lineHeight = font->getHeight();

		float lineX = x;
		float lineY = y + lineHeight;

		int numberOfGlyphs = 0;

		float s = 1.0f / font->getTextureSize();

		for (auto c : text) {
			if (c == '\n') {
				lineX = x;
				lineY += lineHeight;
			}
			else {
				const Glyph* glyph = font->getGlyph(c);

				if (glyph) {
					float xpos = lineX + glyph->left;
					float ypos = glyph->top - lineY - glyph->height;

					float textureLeft = glyph->textureX * s;
					float textureTop = glyph->textureY * s;
					float textureRight = textureLeft + glyph->width * s;
					float textureBottom = textureTop + glyph->height * s;

					vertices[numberOfGlyphs] = {
						xpos, ypos,									textureLeft, textureBottom,
						xpos + glyph->width, ypos,					textureRight, textureBottom,
						xpos, ypos + glyph->height,					textureLeft, textureTop,
						xpos + glyph->width, ypos + glyph->height,	textureRight, textureTop,
						xpos, ypos + glyph->height,					textureLeft, textureTop,
						xpos + glyph->width, ypos,					textureRight, textureBottom,
					};

					numberOfGlyphs++;

					lineX += glyph->advanceX;
				}
			}
		}

		// disable depth testing
		GLboolean depthTest;
		glGetBooleanv(GL_DEPTH_TEST, &depthTest);
		glDisable(GL_DEPTH_TEST);

		// disable depth writing
		GLboolean depthMask;
		glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
		glDepthMask(false);

		// enable blending
		GLboolean blend;
		glGetBooleanv(GL_BLEND, &blend);
		glEnable(GL_BLEND);

		// set correct alpha blending
		GLint blendSrcAlpha, blendDstAlpha;
		glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrcAlpha);
		glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDstAlpha);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		program->use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, font->getTextureId());
		glBindVertexArray(vao);
		glNamedBufferSubData(vbo->getId(), 0, 96 * numberOfGlyphs, &vertices[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6 * numberOfGlyphs);

		// enable depth testing if it was enabled
		if (depthTest == GL_TRUE) {
			glEnable(GL_DEPTH_TEST);
		}

		// restore the previous depth mask
		glDepthMask(depthMask);

		// disable blending if it was disabled
		if (blend == GL_FALSE) {
			glDisable(GL_BLEND);
		}

		// reset alpha blending to the previous setting
		glBlendFunc(blendSrcAlpha, blendDstAlpha);
	}

	void Text::applyWindowSize()
	{
		glm::mat4 projection = glm::ortho(0.0f, (float)windowWidth, -(float)windowHeight, 0.0f);

		program->use();
		program->setUniform("projection", projection);
	}

	void Text::drawFromStream(const StringStream& stream, void* params)
	{
		DrawConfiguration* config = (DrawConfiguration*)params;
		((Text*)config->text)->draw(config->x, config->y, stream.getUtf8String());
	}
}
