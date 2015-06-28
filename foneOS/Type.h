#pragma once

struct FoneFontDesc
{
	FoneOSString Name;
	FoneOSString Style;
};

#define DEFAULT_FONT { STR("Lato"), STR("Regular") }

class Type
{
public:
	// Initializes the type library.
	static bool Init();

	// Forces the font with the specified description to be loaded into the font cache.
	static bool ForceFontLoad(FoneFontDesc desc);

	// Gets the size (in pixels) of the text in the specified font.
	static FT_Vector GetDimensions(FoneFontDesc desc, const char * text, int size);

	// Gets an vector with the specified parameters.
	static std::vector<std::vector<unsigned char>> GetBitmap(FoneFontDesc desc, int size, FoneOSString text, int * WIDTHout, int * HEIGHTout);
};

