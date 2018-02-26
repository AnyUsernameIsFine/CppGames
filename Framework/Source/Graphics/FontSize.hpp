#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL\glew.h>

#include <unordered_map>

namespace Framework
{
	struct Glyph
	{
		int textureX;
		int textureY;
		unsigned int width;
		unsigned int height;
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
		const Glyph* getGlyph(char character);
		const Glyph* addGlyph(char character, FT_GlyphSlot glyph);

	private:
		int size;
		std::unordered_map<char, Glyph> glyphs;
		GLuint textureId;
	};
}
