#pragma once

#include "System\Globals.hpp"
#include "Graphics\FontPacker.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>

namespace Framework
{
	struct Glyph
	{
		int textureX;
		int textureY;
		uInt width;
		uInt height;
		FT_Int left;
		FT_Int top;
		FT_Pos advanceX;
	};

	class FontSize
	{
	public:
		FontSize(int size);
		int getSize() const;
		GLuint getTextureId() const;
		int getTextureSize() const;
		const Glyph* getGlyph(FT_ULong character);
		const Glyph* addGlyph(FT_ULong character, FT_GlyphSlot glyph);

	private:
		int size;
		std::unordered_map<FT_ULong, Glyph> glyphs;
		FontPacker fontPacker;
	};
}
