#pragma once

#include <memory>
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H
#include "Graphics\FontPacker.h"

namespace Framework
{
	class FontSize
	{
	private:
		static constexpr int FONT_PACKER_START_CAPACITY = 1 << 6;
		static constexpr int FONT_PACKER_MAX_CAPACITY = 1 << 10;

		struct Glyph
		{
			int x, y;
			int width, height;
			int left, top;
			int advanceX;
		};

		FontSize(int size);
		int getSize() const;
		int getTextureSize() const;
		const Glyph* getGlyph(UInt32 character) const;
		const Glyph* addGlyph(UInt32 character, FT_GlyphSlot glyph);
		void useTexture() const;

		static std::shared_ptr<FontSize> create(int size);

		int size;
		std::unordered_map<FT_ULong, Glyph> glyphs;
		FontPacker fontPacker;

		friend class Font;
	};
}
