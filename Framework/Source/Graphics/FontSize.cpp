#include "FontSize.hpp"
#include "System\Error.hpp"

namespace Framework
{
	FontSize::FontSize(int size) : fontPacker(64, 1024)
	{
		this->size = size;
	}

	int FontSize::getSize() const
	{
		return size;
	}

	GLuint FontSize::getTextureId() const
	{
		return fontPacker.getTextureId();
	}

	int FontSize::getTextureSize() const
	{
		return fontPacker.getCapacity();
	}

	const Glyph* FontSize::getGlyph(FT_ULong character)
	{
		auto glyph = glyphs.find(character);

		if (glyph != glyphs.end()) {
			return &glyph->second;
		}
		else {
			return nullptr;
		}
	}

	const Glyph* FontSize::addGlyph(FT_ULong character, FT_GlyphSlot glyph)
	{
		int left, top;

		if (fontPacker.addBitmap(glyph->bitmap.width, glyph->bitmap.rows, glyph->bitmap.buffer, left, top)) {
			glyphs[character] = {
				left,
				top,
				glyph->bitmap.width,
				glyph->bitmap.rows,
				glyph->bitmap_left,
				glyph->bitmap_top,
				glyph->advance.x >> 6,
			};

			return &glyphs[character];
		}
		else {
			error("Font texture full");

			return nullptr;
		}
	}
}
