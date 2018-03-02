#include "Text.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace Framework
{
	void Text::loadFont(const string& filename)
	{
		auto font = std::make_shared<Font>();

		if (font->loadFromFile(filename)) {
			if (findFont(font->getFamilyName())) {
				error("Font has already been loaded");
			}
			else {
				fonts[font->getFamilyName()] = font;
			}
		}
	}

	void Text::loadFont(const string& filename, const string& family)
	{
		if (findFont(family)) {
			error("Font has already been loaded");
			return;
		}

		auto font = std::make_shared<Font>();

		if (font->loadFromFile(filename)) {
			fonts[family] = font;
		}
	}

	void Text::setFontFamily(const string& family)
	{
		auto font = findFont(family);

		if (!font) {
			error("Font family not loaded");
			return;
		}

		if (font != this->font) {
			this->font = font;
			this->font->setSize(size);
		}
	}

	void Text::setFontSize(int size)
	{
		if (!font) {
			error("No font set");
			return;
		}

		if (size != this->size) {
			this->size = size;
			font->setSize(size);
		}
	}

	void Text::setFont(const string& family, int size)
	{
		setFontFamily(family);
		setFontSize(size);
	}

	void Text::setColor(float r, float g, float b, float a)
	{
		shader.use();
		shader.setUniform("color", glm::vec4(r, g, b, a));
	}

	int Text::getFontHeight() const
	{
		return font->getHeight();
	}

	void Text::useFontTexture() const
	{
		if (!font) {
			error("No font set");
			return;
		}

		font->useTexture();
	}

	StringStream Text::operator()(float x, float y)
	{
		DrawConfiguration drawConfiguration = { this, x, y };

		return {
			drawFromStreamCallback,
			&drawConfiguration,
			sizeof(DrawConfiguration)
		};
	}

	std::shared_ptr<Font> Text::findFont(const string& family) const
	{
		auto font = fonts.find(family);

		if (font != fonts.end()) {
			return font->second;
		}

		return nullptr;
	}

	void Text::initialize(int windowWidth, int windowHeight)
	{
		this->windowWidth = windowWidth;
		this->windowHeight = windowHeight;

		string vertexShaderSource =
			"#version 330 core\n"
			"layout(location = 0) in vec4 attributes;"
			"out vec2 vertTexCoords;"
			"uniform mat4 projection;"
			"void main()"
			"{"
			"	gl_Position = projection * vec4(attributes.xy, 0, 1);"
			"	vertTexCoords = attributes.zw;"
			"}";

		string fragmentShaderSource =
			"#version 330 core\n"
			"in vec2 vertTexCoords;"
			"out vec4 fragColor;"
			"uniform sampler2D fontSizeTexture;"
			"uniform vec4 color;"
			"void main()"
			"{"
			"	fragColor = color * vec4(1, 1, 1, texture(fontSizeTexture, vertTexCoords).r);"
			"}";

		shader.createFromSource(vertexShaderSource, fragmentShaderSource);

		applyWindowSize();

		setColor(0.5f, 0.5f, 0.5f);

		vertexArray.setVertexBuffer({ 4 }, MAX_STRING_LENGTH * sizeof(GlyphQuad) / sizeof(GLfloat));
	}

	void Text::windowResizedEventHandler(int width, int height)
	{
		windowHasResized = true;

		windowWidth = width;
		windowHeight = height;
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

		float scale = 1.0f / font->getTextureSize();

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

					float left = glyph->x * scale;
					float top = glyph->y * scale;
					float right = left + glyph->width * scale;
					float bottom = top + glyph->height * scale;

					vertices[numberOfGlyphs] = {
						xpos, ypos,									left, bottom,
						xpos + glyph->width, ypos,					right, bottom,
						xpos, ypos + glyph->height,					left, top,
						xpos + glyph->width, ypos + glyph->height,	right, top,
						xpos, ypos + glyph->height,					left, top,
						xpos + glyph->width, ypos,					right, bottom,
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

		shader.use();
		useFontTexture();
		vertexArray.updateVertexBuffer(numberOfGlyphs * sizeof(GlyphQuad) / sizeof(GLfloat), &vertices[0]);
		vertexArray.draw(GL_TRIANGLES);

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

		shader.use();
		shader.setUniform("projection", projection);
	}

	void Text::drawFromStreamCallback(const StringStream& stream, void* data)
	{
		DrawConfiguration* config = (DrawConfiguration*)data;
		((Text*)config->text)->draw(config->x, config->y, stream.getUtf8String());
	}
}
