#include "FontSize.hpp"

#include "System\Error.hpp"

namespace Framework
{
	FontSize::FontSize(int size)
	{
		size_ = size;

		// Generate texture
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &textureId_);
		glBindTexture(GL_TEXTURE_2D, textureId_);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			1024,
			1024,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			nullptr
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	FontSize::~FontSize()
	{
	}

	int FontSize::getSize() const
	{
		return size_;
	}

	GLuint FontSize::getTextureId() const
	{
		return textureId_;
	}

	const Glyph* FontSize::getGlyph(char character)
	{
		bool found = false;
		const Glyph* result = nullptr;

		for (auto i = glyphs_.begin(); i != glyphs_.end() && !found; i++) {
			if (i->character == character) {
				found = true;
				result = &(*i);
			}
		}

		return result;
	}

	const Glyph* FontSize::addGlyph(char character, FT_GlyphSlot glyph)
	{
		glActiveTexture(GL_TEXTURE0);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		int textureX = ((unsigned char)character % 16) * (1024 / 16.0f);
		int textureY = ((unsigned char)character / 16) * (1024 / 16.0f);

		glTextureSubImage2D(
			textureId_,
			0,
			textureX,
			textureY,
			glyph->bitmap.width,
			glyph->bitmap.rows,
			GL_RED,
			GL_UNSIGNED_BYTE,
			glyph->bitmap.buffer
		);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		// Now store character for later use
		glyphs_.push_back({
			character,
			textureX,
			textureY,
			glyph->bitmap.width,
			glyph->bitmap.rows,
			glyph->bitmap_left,
			glyph->bitmap_top,
			glyph->advance.x >> 6,
		});

		return &glyphs_.back();
	}
}
