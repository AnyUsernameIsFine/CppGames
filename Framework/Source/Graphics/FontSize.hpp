#pragma once

#include "Graphics\FontPacker.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>

namespace Framework
{
	struct Glyph
	{
		int x, y;
		uInt width, height;
		int left, top;
		int advanceX;
	};

	class FontSize
	{
	public:
		FontSize(int size);
		int getSize() const;
		GLuint getTextureId() const;
		int getTextureSize() const;
		const Glyph* getGlyph(uInt32 character) const;
		const Glyph* addGlyph(uInt32 character, FT_GlyphSlot glyph);

	private:
		int size;
		std::unordered_map<FT_ULong, Glyph> glyphs;
		FontPacker fontPacker;
	};
}
