#pragma once

#include <memory>

#include "FontSize.h"

namespace Framework
{
	class Font
	{
	private:
		struct GlyphQuad
		{
			GLfloat data[24];
		};

		Font() {}
		~Font();
		bool loadFromFile(const std::string& filename);
		void setSize(int size);
		int getGlyphQuads(int x, int y, const std::u32string& text, std::vector<GlyphQuad>* glyphQuads) const;
		std::string getFamilyName() const;
		int getHeight() const;
		void useTexture() const;
		std::shared_ptr<FontSize> findFontSize(int size) const;

		static std::shared_ptr<Font> create();

		FT_Library freeType;
		FT_Face face = nullptr;
		std::vector<std::shared_ptr<FontSize>> fontSizes;
		std::shared_ptr<FontSize> fontSize = nullptr;
		int verticalCenterOffset = 0;

		friend class Text;
	};
}
