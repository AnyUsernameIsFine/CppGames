#include "Text.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\glm.hpp>

namespace Framework
{
	void Text::loadFont(const std::string& filename)
	{
		auto font = Font::create();

		if (font->loadFromFile(filename)) {
			if (findFont(font->getFamilyName())) {
				error("Font has already been loaded");
			}
			else {
				fonts[font->getFamilyName()] = font;
			}
		}
	}

	void Text::loadFont(const std::string& filename, const std::string& family)
	{
		if (findFont(family)) {
			error("Font has already been loaded");
			return;
		}

		auto font = Font::create();

		if (font->loadFromFile(filename)) {
			fonts[family] = font;
		}
	}

	void Text::setFontFamily(const std::string& family)
	{
		auto font = findFont(family);

		if (!font) {
			error("Font family not loaded");
			return;
		}

		if (font != this->font) {
			if (size) {
				font->setSize(size);
			}
			this->font = font;
		}
	}

	void Text::setFontSize(int size)
	{
		if (size != this->size) {
			if (font) {
				font->setSize(size);
			}
			this->size = size;
		}
	}

	void Text::setFont(const std::string& family, int size)
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
		if (!font) {
			error("No font set");
			return 0;
		}

		return font->getHeight();
	}

	void Text::useFontTexture() const
	{
		if (!font) {
			error("No font set");
			return;
		}

		if (!size) {
			error("No font size set");
			return;
		}

		font->useTexture();
	}

	StringStream Text::draw(int x, int y)
	{
		DrawConfiguration drawConfiguration = { this, x, y };

		return {
			drawFromStreamCallback,
			&drawConfiguration,
			sizeof(drawConfiguration)
		};
	}

	std::shared_ptr<Font> Text::findFont(const std::string& family) const
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

		std::string vertexShaderSource =
			"#version 330 core\n"
			"layout(location = 0) in vec4 attributes;"
			"out vec2 vertTexCoords;"
			"uniform mat4 projection;"
			"void main()"
			"{"
			"	gl_Position = projection * vec4(attributes.xy, 0, 1);"
			"	vertTexCoords = attributes.zw;"
			"}";

		std::string fragmentShaderSource =
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

		vertexArray.setVertexBuffer({ 4 }, MAX_STRING_LENGTH * sizeof(Font::GlyphQuad) / sizeof(GLfloat));
	}

	void Text::windowResizedEventHandler(int width, int height)
	{
		windowHasResized = true;

		windowWidth = width;
		windowHeight = height;
	}

	void Text::draw(int x, int y, const std::u32string& text)
	{
		if (!font) {
			error("No font set");
			return;
		}

		if (!size) {
			error("No font size set");
			return;
		}

		if (windowHasResized) {
			windowHasResized = false;
			applyWindowSize();
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
		int numVertices = font->getGlyphQuads(x, y, text, &glyphQuads);
		vertexArray.updateVertexBuffer(numVertices, glyphQuads.data());
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
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowWidth), -static_cast<float>(windowHeight), 0.0f);

		shader.use();
		shader.setUniform("projection", projection);
	}

	void Text::drawFromStreamCallback(const StringStream& stream, void* data)
	{
		DrawConfiguration* config = static_cast<DrawConfiguration*>(data);
		static_cast<Text*>(config->text)->draw(config->x, config->y, stream.getUtf8String());
	}
}
