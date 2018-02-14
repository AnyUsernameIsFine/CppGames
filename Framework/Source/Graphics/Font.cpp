#include "Font.hpp"
#include "System\Error.hpp"

namespace Framework
{
	Font::Font(const std::string& filename, int size)
	{
		// FreeType
		FT_Library ft;
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&ft))
			error("ERROR::FREETYPE: Could not init FreeType Library");

		// Load font as face
		FT_Face face;
		if (FT_New_Face(ft, filename.c_str(), 0, &face))
			error("ERROR::FREETYPE: Failed to load font");

		// Set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, size);

		height_ = face->size->metrics.height >> 6;

		glActiveTexture(GL_TEXTURE0);

		// Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Load first 128 characters of ASCII set
		for (int c = 0; c < face->num_glyphs; c++) {
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				error("ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}
			//output(static_cast<char>(c));
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Glyph_ glyph = {
				texture,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				face->glyph->bitmap_left,
				face->glyph->bitmap_top,
				face->glyph->advance.x >> 6,
			};
			glyphs_.insert(std::pair<FT_ULong, Glyph_>(c, glyph));
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));

		// Destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}
}
