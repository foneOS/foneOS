#pragma once
class Display
{
public:
	virtual ~Display() { }

	// Initializes the display.
	virtual void Init() = 0;

	// Clears the screen with the specified color.
	virtual void Clear(FoneOSColor color) = 0;

	// Draws a rectangle.
	virtual void DrawRectangle(int x, int y, int w, int h, FoneOSColor color) = 0;

	// Fills a rectangle.
	virtual void FillRectangle(int x, int y, int w, int h, FoneOSColor color) = 0;

	// Draws an image to the specified position. Returns true on success.
	virtual bool DrawImage(FoneOSString filename, int x, int y) = 0;

	// Draws a string of characters to the specified position.
	virtual void DrawString(FoneOSString string, int x, int y, FoneFontDesc font, int size, FoneOSColor color, FoneOSColor bg) = 0;

	// Draws a string of characters to the specified position.
	virtual void DrawString(FoneOSString string, int x, int y, int size, FoneOSColor color, FoneOSColor bg) = 0;

	// Flushes all data to the display.
	virtual void Flush() = 0;

	// Updates the display.
	virtual void Update() = 0;

	// Frees memory associated with the display.
	virtual void Cleanup() = 0;

	// Get's the display's horizontal dots-per-inch.
	virtual int GetHorizDPI() = 0;

	// Get's the display's vertical dots-per-inch.
	virtual int GetVertDPI() = 0;

	/*static const int HorizDPI = 72;
	static const int VertDPI = 72;*/
};

class Display_SDL : public Display
{
	virtual void Init();
	virtual void Clear(FoneOSColor color);
	virtual void DrawRectangle(int x, int y, int w, int h, FoneOSColor color);
	virtual void FillRectangle(int x, int y, int w, int h, FoneOSColor color);
	virtual bool DrawImage(FoneOSString filename, int x, int y);
	virtual void DrawString(FoneOSString string, int x, int y, FoneFontDesc font, int size, FoneOSColor color, FoneOSColor bg);
	virtual void DrawString(FoneOSString string, int x, int y, int size, FoneOSColor color, FoneOSColor bg);
	virtual void Flush();
	virtual void Update();
	virtual void Cleanup();
	virtual int GetHorizDPI();
	virtual int GetVertDPI();
};