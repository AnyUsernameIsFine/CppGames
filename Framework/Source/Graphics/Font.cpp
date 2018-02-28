#include "Font.hpp"
#include "System\Error.hpp"

namespace Framework
{
	int Font::loadFromFile(const string& filename)
	{
		if (!face) {
			if (FT_Init_FreeType(&freeType)) {
				error("Could not initialize FreeType");

				return 1;
			}

			if (FT_New_Face(freeType, filename.c_str(), 0, &face)) {
				error("Could not load font from file " + filename);

				return 1;
			}
		}

		return 0;
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
				this->fontSize = fontSize.get();
			}
		}
	}

	const string Font::getFamilyName() const
	{
		return face->family_name;
	}

	FT_Pos Font::getHeight() const
	{
		return face->size->metrics.height >> 6;
	}

	const Glyph* Font::getGlyph(FT_ULong character) const
	{
		const Glyph* glyph = fontSize->getGlyph(character);

		if (!glyph) {
			if (FT_Load_Char(face, character, FT_LOAD_RENDER)) {
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

	FontSize* Font::findFontSize(int size) const
	{
		bool found = false;
		FontSize* result = nullptr;

		for (auto i = fontSizes.begin(); i != fontSizes.end() && !found; i++) {
			if (i->get()->getSize() == size) {
				found = true;
				result = i->get();
			}
		}

		return result;
	}
}
