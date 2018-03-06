#include "FontSize.h"

namespace GLEngine
{
	FontSize::FontSize(int size) : fontPacker(FONT_PACKER_START_CAPACITY, FONT_PACKER_MAX_CAPACITY)
	{
		this->size = size;
	}

	int FontSize::getSize() const
	{
		return size;
	}

	int FontSize::getTextureSize() const
	{
		return fontPacker.getCapacity();
	}

	const FontSize::Glyph* FontSize::getGlyph(UInt32 character) const
	{
		auto glyph = glyphs.find(character);

		if (glyph != glyphs.end()) {
			return &glyph->second;
		}
		else {
			return nullptr;
		}
	}

	const FontSize::Glyph* FontSize::addGlyph(UInt32 character, const FT_GlyphSlot& glyph)
	{
		int x, y;

		if (fontPacker.addBitmap(glyph->bitmap.width, glyph->bitmap.rows, glyph->bitmap.buffer, &x, &y)) {
			glyphs[character] = {
				x,
				y,
				static_cast<int>(glyph->bitmap.width),
				static_cast<int>(glyph->bitmap.rows),
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

	void FontSize::useTexture() const
	{
		fontPacker.useTexture();
	}

	std::shared_ptr<FontSize> FontSize::create(int size)
	{
		struct MakeSharedEnabler : public FontSize
		{
			MakeSharedEnabler(int size) : FontSize(size) {}
		};

		return std::make_shared<MakeSharedEnabler>(size);
	}
}
