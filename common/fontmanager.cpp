#include "fontmanager.h"

FontManager::FontManager()
{

}

FontManager::~FontManager()
{

}

std::map<char, glyph*> FontManager::GetFont(const char* fontPath, int size)
{
	if (_fonts[std::make_pair(fontPath, size)][0] != nullptr)
	{
		return _fonts[std::make_pair(fontPath, size)];
	}
	MakeFont(fontPath, size);
	return _fonts[std::make_pair(fontPath, size)];
}

void FontManager::MakeFont(const char* fontPath, int size)
{
	FT_Library ft;

	std::cout << "Loading font: " << fontPath << "\n";

	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << "\n";
		return;
	}
	else { std::cout << "FREETYPE: init freetype complete" << "\n"; }

	FT_Face fontFace;
	if (FT_New_Face(ft, fontPath, 0, &fontFace))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << "\n";
		return;
	}

	else
	{
		std::cout << "FREETYPE: loaded freetype font" << "\n";
	}

	// Set the font size
	FT_Set_Pixel_Sizes(fontFace, 0, size);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_GlyphSlot slot = fontFace->glyph;
	// create temp chars list
	std::map<char, glyph*> chars;
	for (GLubyte c = 0; c < 128; c++) { // Load first 128 characters of ASCII set
		// Load character glyph
		if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << "\n";
			continue;
		}

		FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, fontFace->glyph->bitmap.buffer);

		// Now store character for later use
		glyph* ch = new glyph{
			texture,
			glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
			glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
			GLuint(fontFace->glyph->face->bbox.yMax),
			GLuint(fontFace->glyph->advance.x)
		};

		// add char to chars list
		chars[c] = ch;
		// Unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	_fonts[std::make_pair(fontPath, size)] = chars;
	std::cout << "FREETYPE: font loaded" << "\n";


	// Destroy FreeType once we're finished
	FT_Done_Face(fontFace);
	FT_Done_FreeType(ft);
}