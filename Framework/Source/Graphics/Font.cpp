#include "Font.hpp"
#include "System\Error.hpp"

namespace Framework
{
	Font::Font(const std::string& filename)
	{
		if (FT_Init_FreeType(&freeType_)) {
			error("Could not initialize FreeType");
		}

		if (FT_New_Face(freeType_, filename.c_str(), 0, &face_)) {
			error("Could not load font from file " + filename);
		}
	}

	Font::~Font()
	{
		FT_Done_Face(face_);
		FT_Done_FreeType(freeType_);
	}

	void Font::setSize(int size)
	{
		if (!fontSize_ || (fontSize_ && size != fontSize_->getSize())) {
			fontSize_ = findFontSize_(size);

			if (fontSize_) {
				if (size != face_->size->metrics.y_ppem) {
					FT_Set_Pixel_Sizes(face_, 0, size);
				}
			}
			else {
				if (FT_Set_Pixel_Sizes(face_, 0, size)) {
					error("Could not set font size " + size);
				}

				auto fontSize = std::make_shared<FontSize>(size);
				fontSizes_.push_back(fontSize);
				fontSize_ = fontSize.get();
			}
		}
	}

	FT_String* Font::getFamilyName() const
	{
		return face_->family_name;
	}

	FT_Pos Font::getHeight() const
	{
		return face_->size->metrics.height >> 6;
	}

	const Glyph* Font::getGlyph(char character) const
	{
		const Glyph* glyph = fontSize_->getGlyph(character);

		if (!glyph) {
			if (FT_Load_Char(face_, character, FT_LOAD_RENDER)) {
				error("Could not load glyph " + character);
			}

			glyph = fontSize_->addGlyph(character, face_->glyph);
		}

		return glyph;
	}

	GLuint Font::getTextureId() const
	{
		return fontSize_->getTextureId();
	}

	FontSize* Font::findFontSize_(int size) const
	{
		bool found = false;
		FontSize* result = nullptr;

		for (auto i = fontSizes_.begin(); i != fontSizes_.end() && !found; i++) {
			if (i->get()->getSize() == size) {
				found = true;
				result = i->get();
			}
		}

		return result;
	}
}
