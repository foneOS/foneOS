#pragma once
class Display
{
public:
	// Initializes the display.
	static void Init();

	// Clears the screen with the specified color.
	static void Clear(FoneOSColor color);

	// Draws a rectangle.
	static void DrawRectangle(int x, int y, int w, int h, FoneOSColor color);

	// Fills a rectangle.
	static void FillRectangle(int x, int y, int w, int h, FoneOSColor color);

	// Draws an image to the specified position. Returns true on success.
	static bool DrawImage(FoneOSString filename, int x, int y);

	// Draws a string of characters to the specified position.
	static void DrawString(FoneOSString string, int x, int y, FoneFontDesc font, int size, FoneOSColor color, FoneOSColor bg);

	// Draws a string of characters to the specified position.
	static void DrawString(FoneOSString string, int x, int y, int size, FoneOSColor color, FoneOSColor bg);

	// Flushes all data to the display.
	static void Flush();

	// Updates the display.
	static void Update();

	// Frees memory associated with the display.
	static void Cleanup();

	static const int HorizDPI = 72;
	static const int VertDPI = 72;
};