#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL\glew.h>

#include <vector>

namespace Framework
{
	struct Glyph
	{
		char character;
		GLuint texture;
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
		~FontSize();
		int getSize() const;
		const Glyph* getGlyph(char character);
		const Glyph* addGlyph(char character, FT_GlyphSlot glyph);

	private:
		int size_;
		std::vector<Glyph> glyphs_;
	};
}
