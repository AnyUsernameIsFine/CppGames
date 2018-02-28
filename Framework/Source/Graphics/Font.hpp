#pragma once

#include "System\Globals.hpp"
#include "FontSize.hpp"

#include <memory>

namespace Framework
{
	class Font
	{
	public:
		~Font();
		int loadFromFile(const string& filename);
		void setSize(int size);
		const string getFamilyName() const;
		FT_Pos getHeight() const;
		const Glyph* getGlyph(FT_ULong character) const;
		GLuint getTextureId() const;
		int getTextureSize() const;

	private:
		FT_Library freeType;
		FT_Face face = nullptr;
		vector<std::shared_ptr<FontSize>> fontSizes;
		FontSize* fontSize = nullptr;

		FontSize* findFontSize(int size) const;
	};
}
