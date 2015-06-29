#include "stdafx.h"
#include "Type.h"

struct FoneFontDescComparer
{
	bool operator()(const FoneFontDesc & Left, const FoneFontDesc & Right) const
	{
		return Left.Name < Right.Name || Left.Style < Right.Style;
	}
};

FT_Library library;
std::map<FoneFontDesc, FT_Face, FoneFontDescComparer> fonts;

bool Type::Init()
{
	// TODO: Handle errors
	FT_Init_FreeType(&library);

	if (!Storage::FileExists(STR("fonts/Lato/Regular.ttf")))
	{
		Logging::LogMessage(STR("Could not find Lato font!"));
		return false;
	}

	return Type::ForceFontLoad(DEFAULT_FONT);
}

bool Type::ForceFontLoad(FoneFontDesc desc)
{
	fonts.insert(std::pair<FoneFontDesc, FT_Face>(desc, NULL));
	FoneOSString basePath = FoneOSString(STR("fonts/")) + desc.Name + FoneOSString(STR("/")) + desc.Style + FoneOSString(STR(".ttf"));
	FoneOSString fullPath = Storage::GetFullPath(basePath);
	char * fullPath_c = Utils::FoneOSStringToCharArray(fullPath);
	FT_Error err = FT_New_Face(library,
		fullPath_c,
		0,
		&fonts[desc]);
	free(fullPath_c);
	return (err == 0);
}

/* origin is the upper left corner */
std::vector<std::vector<unsigned char>> image;

void draw_bitmap(FT_Bitmap*  bitmap, FT_Int x, FT_Int y, int WIDTH, int HEIGHT)
{
	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;

	for (i = x, p = 0; i < x_max; i++, p++)
	{
		for (j = y, q = 0; j < y_max; j++, q++)
		{
			if (i < 0 || j < 0 ||
				i >= WIDTH || j >= HEIGHT)
				continue;

			(image[j])[i] |= bitmap->buffer[q * bitmap->width + p];
		}
	}
}

FT_Vector Type::GetDimensions(FoneFontDesc desc, const char * text, int size)
{
	FT_Face face = fonts[desc];

	FT_Error error = FT_Set_Char_Size(
		face,		 /* handle to face object */
		0,			 /* char_width in 1/64th of points */
		size * 64,	 /* char_height in 1/64th of points */
		Display::HorizDPI,		 /* horizontal device resolution */
		Display::VertDPI);		 /* vertical device resolution */

	int stringWidth = 0;
	int stringHeight = 0;
	FT_UInt previous = 0;
	int newlines = 0;
	for (unsigned int n = 0; n < strlen(text); n++)
	{
		if (text[n] == '\n')
		{
			newlines++;
			continue;
		}
		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
		if (error)
			continue;  /* ignore errors */
		FT_UInt glyph_index = FT_Get_Char_Index(fonts[desc], text[n]);
		FT_Vector kerning;
		int kernx = 0;
		if (previous) {
			FT_Get_Kerning(face,	/* handle to face object	*/
				previous,			/* left glyph index			*/
				glyph_index,		/* right glyph index		*/
				FT_KERNING_DEFAULT, /* kerning mode				*/
				&kerning);
			kernx = kerning.x;
		}
		stringWidth += (face->glyph->advance.x + kernx) >> 6;
		FT_BBox bbox;
		FT_Glyph glyph;
		FT_Get_Glyph(face->glyph, &glyph);
		FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &bbox);
		if (bbox.yMax - bbox.yMin > stringHeight)
		{
			stringHeight = bbox.yMax - bbox.yMin;
		}
		FT_Done_Glyph(glyph);
		previous = glyph_index;
	}

	return { stringWidth, stringHeight };
}

std::vector<std::vector<unsigned char>> Type::GetBitmap(FoneFontDesc desc, int size, FoneOSString string, int * WIDTHout, int * HEIGHTout)
{
	if (fonts.find(desc) == fonts.end())
	{
		Type::ForceFontLoad(desc);
	}
	FT_Face face = fonts[desc];
	FT_Error error = FT_Set_Char_Size(
		face,		 /* handle to face object */
		0,			 /* char_width in 1/64th of points */
		size * 64,	 /* char_height in 1/64th of points */
		Display::HorizDPI,		 /* horizontal device resolution */
		Display::VertDPI);		 /* vertical device resolution */

	FT_GlyphSlot	slot = face->glyph;  /* a small shortcut */
	int				pen_x, pen_y, n;
	const char *	text = Utils::FoneOSStringToCharArray(string);
	int				num_chars = strlen(text);

	int WIDTH = 320;
	int HEIGHT = 320;

	FT_Vector dims = Type::GetDimensions(desc, text, size);

	WIDTH = dims.x;
	HEIGHT = dims.y + (dims.y * (Utils::NumberOfNewlines(Utils::CharArrayToFoneOSString((char*)text)) + (size / 12)));

	pen_x = 0;
	pen_y = dims.y;

	image = std::vector<std::vector<unsigned char>>(HEIGHT, std::vector<unsigned char>(WIDTH));

	for (n = 0; n < num_chars; n++)
	{
		if (text[n] == '\n')
		{
			pen_x = 0;
			pen_y += dims.y;
			pen_y += size / 12;
			continue;
		}
		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
		if (error)
			continue;  /* ignore errors */

		FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);

		/* now, draw to our target surface */
		draw_bitmap(&slot->bitmap,
			pen_x + slot->bitmap_left,
			pen_y - slot->bitmap_top,
			WIDTH,
			HEIGHT);

		/* increment pen position */
		pen_x += slot->advance.x >> 6;
	}

	*WIDTHout = WIDTH;
	*HEIGHTout = HEIGHT;
	free((void *)text);
	return image;
}