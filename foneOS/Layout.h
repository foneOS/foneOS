#pragma once

class FoneOSContainer {
public:
	int x = 0;
	int y = 0;

	int width = 0;
	int height = 0;

	FoneOSColor bgColor = COLOR_WHITE;
	FoneOSColor fgColor = COLOR_BLACK;

	void(*onActivate)(FoneOSContainer * cont) = 0;

	virtual void handleTouch(FoneOSPoint point);

	virtual void Draw(FoneOSContainer * scr);

	virtual void Create();
private:
};

class FoneOSLabel : public FoneOSContainer {
public:
	FoneOSLabel();
	FoneOSLabel(FoneOSString str, int xPos, int yPos);

	virtual void Draw(FoneOSContainer * scr);

	int fontSize = 2;
	FoneFontDesc font;

	FoneOSString text;
private:
};


class FoneOSTitle : public FoneOSContainer {
public:
	FoneOSColor bgColor = COLOR_GRAY2;
	FoneOSColor fgColor = COLOR_WHITE;

	int width = 240;
	int height = 25;

	virtual void Draw(FoneOSContainer * scr);

	FoneOSString text;
private:
};

class FoneOSButton : public FoneOSContainer {
public:
	FoneOSButton();
	FoneOSButton(FoneOSString caption, int xPosition, int yPosition);

	void handleTouch(FoneOSPoint point);
	void Draw(FoneOSContainer * scr);
	void Create();

	int fontSize = 2;
	FoneOSString text;
private:
};

class FoneOSImage : public FoneOSContainer {
public:
	virtual void Draw(FoneOSContainer * scr);

	int width = 100;
	int height = 100;

	FoneOSString path;
private:
};

class FoneOSKeyboard : public FoneOSContainer {
public:
	FoneOSKeyboard();

	virtual void handleTouch(FoneOSPoint p);
	virtual void Draw(FoneOSContainer * scr);

	int x = 0;
	int y = 160;
	int width = 240;
	int height = 160;

	FoneOSString path;

	std::vector<FoneOSButton*> topRow;
private:
};


class FoneOSScreen {
public:
	FoneOSColor bgColor = COLOR_WHITE;
	FoneOSColor fgColor = COLOR_BLACK;

	void handleTouch(FoneOSPoint point);

	void Draw();

	FoneOSContainer root = FoneOSContainer();

	std::vector<FoneOSButton> buttons = {};
	std::vector<FoneOSTitle> titles = {};
	std::vector<FoneOSLabel> labels = {};
	std::vector<FoneOSImage> images = {};
private:
};


class Layout {
public:
	// Initializes the layout component.
	static void Init();

	// Draws the layout.
	static void Draw();

	// Updates the layout.
	static void Update();

	// Gets the current layout.
	static FoneOSScreen * GetCurrentLayout(FoneOSScreen * layout);

	// Changes the current layout.
	static void SetCurrentLayout(FoneOSScreen * layout, bool addLastToStack);
private:
	// Draws the action bar.
	static void DrawActionBar();
};