#include "Font.h"

#include <iomanip>
#include <sstream>

namespace GLEngine
{
	bool Font::loadFromFile(const std::string& filename)
	{
		if (face) {
			error("Font has already been loaded");
			return false;
		}

		if (FT_Init_FreeType(&freeType) != 0) {
			error("Could not initialize FreeType");
			return false;
		}

		if (FT_New_Face(freeType, filename.c_str(), 0, &face) != 0) {
			error("Could not load font from file " + filename);
			return false;
		}

		return true;
	}

	Font::~Font()
	{
		FT_Done_Face(face);
		FT_Done_FreeType(freeType);
	}

	void Font::setSize(int size)
	{
		if (!fontSize || (fontSize && size != fontSize->getSize())) {
			fontSize = findFontSize(size);

			if (fontSize) {
				if (size != face->size->metrics.y_ppem) {
					FT_Set_Pixel_Sizes(face, 0, size);
				}
			}
			else {
				if (FT_Set_Pixel_Sizes(face, 0, size)) {
					error("Could not set font size " + size);
				}

				auto fontSize = FontSize::create(size);
				fontSizes.push_back(fontSize);
				this->fontSize = fontSize;
			}

			if (FT_Load_Char(face, 'x', FT_LOAD_RENDER) != 0) {
				error("Could not determine vertical center offset");
			}

			verticalCenterOffset = (face->size->metrics.height >> 7) - (face->glyph->bitmap_top >> 1);
		}
	}

	int Font::getGlyphQuads(int x, int y, const std::u32string& text, std::vector<GlyphQuad>* glyphQuads) const
	{
		float lineX = static_cast<float>(x);
		float lineY = static_cast<float>(y + getHeight() - verticalCenterOffset);

		int numGlyphs = 0;

		float scale = 1.0f / fontSize->getTextureSize();

		for (const auto& character : text) {
			if (character == '\n') {
				lineX = static_cast<float>(x);
				lineY += getHeight();
			}
			else {
				const FontSize::Glyph* glyph = fontSize->getGlyph(character);

				if (!glyph) {
					if (FT_Load_Char(face, character, FT_LOAD_RENDER) != 0) {
						std::stringstream stream;
						stream << "Could not load UTF-8 character U+" << std::setfill('0') <<
							std::setw(4) << std::hex << character << " (" << character << ")";
						error(stream.str());
					}

					glyph = fontSize->addGlyph(character, face->glyph);
				}

				if (glyph) {
					float xpos = lineX + glyph->left;
					float ypos = glyph->top - lineY - glyph->height;

					float left = glyph->x * scale;
					float top = glyph->y * scale;
					float right = left + glyph->width * scale;
					float bottom = top + glyph->height * scale;

					(*glyphQuads)[numGlyphs] = {
						xpos, ypos, left, bottom,
						xpos + glyph->width, ypos, right, bottom,
						xpos, ypos + glyph->height, left, top,
						xpos + glyph->width, ypos + glyph->height, right, top,
						xpos, ypos + glyph->height, left, top,
						xpos + glyph->width, ypos, right, bottom,
					};

					numGlyphs++;

					lineX += glyph->advanceX;
				}
			}
		}

		return numGlyphs * 6;
	}

	std::string Font::getFamilyName() const
	{
		return face->family_name;
	}

	int Font::getHeight() const
	{
		return face->size->metrics.height >> 6;
	}

	void Font::useTexture() const
	{
		return fontSize->useTexture();
	}

	std::shared_ptr<FontSize> Font::findFontSize(int size) const
	{
		for (const auto& fontSize : fontSizes) {
			if (fontSize->getSize() == size) {
				return fontSize;
			}
		}

		return nullptr;
	}

	std::shared_ptr<Font> Font::create()
	{
		struct MakeSharedEnabler : public Font
		{
			MakeSharedEnabler() : Font() {}
		};

		return std::make_shared<MakeSharedEnabler>();
	}
}
