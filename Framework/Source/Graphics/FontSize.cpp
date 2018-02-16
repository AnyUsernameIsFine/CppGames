#include "FontSize.hpp"

#include "System\Error.hpp"

namespace Framework
{
	FontSize::FontSize(int size)
	{
		size_ = size;
	}

	FontSize::~FontSize()
	{
	}

	int FontSize::getSize() const
	{
		return size_;
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
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			glyph->bitmap.width,
			glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		glyphs_.push_back({
			character,
			texture,
			glyph->bitmap.width,
			glyph->bitmap.rows,
			glyph->bitmap_left,
			glyph->bitmap_top,
			glyph->advance.x >> 6,
		});

		glBindTexture(GL_TEXTURE_2D, 0);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		return &glyphs_.back();
	}
}
