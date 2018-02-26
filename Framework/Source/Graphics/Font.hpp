#pragma once

#include "System\Globals.hpp"
#include "FontSize.hpp"

#include <memory>

namespace Framework
{
	class Font
	{
	public:
		Font(const string& filename);
		~Font();
		void setSize(int size);
		FT_String* getFamilyName() const;
		FT_Pos getHeight() const;
		const Glyph* getGlyph(char character) const;
		GLuint getTextureId() const;

	private:
		FT_Library freeType;
		FT_Face face;
		vector<std::shared_ptr<FontSize>> fontSizes;
		FontSize* fontSize = nullptr;

		FontSize* findFontSize(int size) const;
	};
}
