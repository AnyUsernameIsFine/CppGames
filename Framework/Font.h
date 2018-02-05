#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL\glew.h>

#include <string>
#include <map>

namespace Framework
{
	class Font
	{
	public:
		Font(const std::string& filename, unsigned int size);

	private:
		friend class Text;

		struct Glyph_
		{
			GLuint textureId;
			unsigned int width;
			unsigned int height;
			FT_Int left;
			FT_Int top;
			FT_Pos advanceX;
		};

		FT_Pos height_;
		std::map<FT_ULong, Glyph_> glyphs_;
	};
}
