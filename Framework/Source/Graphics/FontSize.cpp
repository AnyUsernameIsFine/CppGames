#include "FontSize.h"

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

	void FontSize::useTexture() const
	{
		fontPacker.useTexture();
	}

	int FontSize::getTextureSize() const
	{
		return fontPacker.getCapacity();
	}

	const Glyph* FontSize::getGlyph(uInt32 character) const
	{
		auto glyph = glyphs.find(character);

		if (glyph != glyphs.end()) {
			return &glyph->second;
		}
		else {
			return nullptr;
		}
	}

	const Glyph* FontSize::addGlyph(uInt32 character, FT_GlyphSlot glyph)
	{
		int x, y;

		if (fontPacker.addBitmap(glyph->bitmap.width, glyph->bitmap.rows, glyph->bitmap.buffer, x, y)) {
			glyphs[character] = {
				x,
				y,
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
