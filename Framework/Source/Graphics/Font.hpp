#pragma once

#include "FontSize.hpp"

#include <string>
#include <memory>
#include <vector>

namespace Framework
{
	class Font
	{
	public:
		Font(const std::string& filename);
		~Font();
		void setSize(int size);
		FT_String* getFamilyName() const;
		FT_Pos getHeight() const;
		const Glyph* getGlyph(char character) const;
		GLuint getTextureId() const;

	private:
		FT_Library freeType_;
		FT_Face face_;
		std::vector<std::shared_ptr<FontSize>> fontSizes_;
		FontSize* fontSize_ = nullptr;

		FontSize* findFontSize_(int size) const;
	};
}
