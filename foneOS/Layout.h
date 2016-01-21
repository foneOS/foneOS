#pragma once

class FoneOSScreen;\

class FoneOSContainer {
public:
	FoneOSString id = STR("");

	int x = 0;
	int y = 0;

	int width = 0;
	int height = 0;

	FoneOSColor bgColor = COLOR_WHITE;
	FoneOSColor fgColor = COLOR_BLACK;

	std::map<std::string, void*> metadata = std::map<std::string, void*>();

	void(*onActivate)(FoneOSContainer * cont) = 0;
	
	virtual ~FoneOSContainer() {}
	
	virtual void handleTouch(FoneOSPoint point);

    virtual void Draw(FoneOSScreen * scr);

	virtual void Create();
private:
};

class FoneOSLabel : public FoneOSContainer {
public:	FoneOSLabel();
	FoneOSLabel(FoneOSString str, int xPos, int yPos);

    virtual void Draw(FoneOSScreen * scr);

	virtual void Create();

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

    virtual void Draw(FoneOSScreen * scr);

	FoneOSString text;
private:
};

class FoneOSButton : public FoneOSContainer {
public:
	FoneOSButton();
	FoneOSButton(FoneOSString caption, int xPosition, int yPosition);

	void handleTouch(FoneOSPoint point);
    void Draw(FoneOSScreen * scr);
	void Create();

	int fontSize = 2;
	FoneOSString text;
private:
};

class FoneOSImage : public FoneOSContainer {
public:
    virtual void Draw(FoneOSScreen * scr);

	int width = 100;
	int height = 100;

	FoneOSString path;
private:
};

class FoneOSKeyboard : public FoneOSContainer {
public:
	FoneOSKeyboard();

	virtual void handleTouch(FoneOSPoint p);
    virtual void Draw(FoneOSScreen * scr);

	int x = 0;
	int y = 160;
	int width = 240;
	int height = 160;

	FoneOSString path;

	FoneOSButton topRow[10];
	FoneOSButton middleRow[9];
	FoneOSButton bottomRow[7];
	FoneOSButton space;
private:
};


class FoneOSScreen {
public:
	FoneOSScreen();

	FoneOSColor bgColor = COLOR_WHITE;
	FoneOSColor fgColor = COLOR_BLACK;

	void handleTouch(FoneOSPoint point);

	void Draw();

	FoneOSContainer root = FoneOSContainer();

	int width = 0;
	int height = 0;

	std::vector<FoneOSButton *> buttons = {};
	std::vector<FoneOSTitle *> titles = {};
	std::vector<FoneOSLabel *> labels = {};
	std::vector<FoneOSImage *> images = {};
	std::vector<FoneOSKeyboard *> keyboards = {};
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
	static FoneOSScreen * GetCurrentLayout();

	// Changes the current layout.
	static void SetCurrentLayout(FoneOSScreen * layout, bool addLastToStack);
private:
	// Draws the action bar.
	static void DrawActionBar();
};
