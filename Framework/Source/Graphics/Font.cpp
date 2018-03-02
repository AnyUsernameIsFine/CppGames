#include "Font.hpp"

namespace Framework
{
	bool Font::loadFromFile(const string& filename)
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

				auto fontSize = std::make_shared<FontSize>(size);
				fontSizes.push_back(fontSize);
				this->fontSize = fontSize;
			}
		}
	}

	string Font::getFamilyName() const
	{
		return face->family_name;
	}

	int Font::getHeight() const
	{
		return face->size->metrics.height >> 6;
	}

	const Glyph* Font::getGlyph(uInt32 character) const
	{
		const Glyph* glyph = fontSize->getGlyph(character);

		if (!glyph) {
			if (FT_Load_Char(face, character, FT_LOAD_RENDER) != 0) {
				error("Could not load glyph " + character);
			}

			glyph = fontSize->addGlyph(character, face->glyph);
		}

		return glyph;
	}

	GLuint Font::getTextureId() const
	{
		return fontSize->getTextureId();
	}

	int Font::getTextureSize() const
	{
		return fontSize->getTextureSize();
	}

	std::shared_ptr<FontSize> Font::findFontSize(int size) const
	{
		for (auto& fontSize : fontSizes) {
			if (fontSize->getSize() == size) {
				return fontSize;
			}
		}

		return nullptr;
	}
}
