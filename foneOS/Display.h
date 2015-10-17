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

    // Gets the display's horizontal dots-per-inch.
	virtual int GetHorizDPI() = 0;

    // Gets the display's vertical dots-per-inch.
	virtual int GetVertDPI() = 0;

    // Gets the display's width in pixels.
    virtual int GetWidth() = 0;

    // Gets the display's height in pixels.
    virtual int GetHeight() = 0;

	/*static const int HorizDPI = 72;
	static const int VertDPI = 72;*/
};

#ifdef SIMULATOR_BUILD
class Display_SDL : public Display
{
public:
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
    virtual int GetWidth();
    virtual int GetHeight();
private:
	SDL_Surface * buf;
};
#endif

#ifdef PRODUCTION_BUILD
class Display_ST7781R : public Display
{
public:
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
    virtual int GetWidth();
    virtual int GetHeight();
private:
    std::map<unsigned int, mraa_gpio_context> pins;
    std::map<unsigned int, mraa_aio_context> apins;

    void SetPin(unsigned int pin, mraa_gpio_dir_t dir);
    void SetAPin(unsigned int pin);

    void DigitalWrite(unsigned int pin, unsigned int value);
    int DigitalRead(unsigned int pin);
    int AnalogRead(unsigned int pin);

    void AllPinLow();
    void AllPinOutput();

    void PushData(unsigned char data);
    unsigned char GetData();
    void SendCommand(unsigned int index);
    void SendData(unsigned int data);
    unsigned int ReadRegister(unsigned int index);

    void ExitStandBy();

    uint16_t CreateColor(FoneOSColor color);

    void DrawHorizontalLine(unsigned int x, unsigned int y, unsigned int length, FoneOSColor color);
    void DrawVerticalLine(unsigned int x, unsigned int y, unsigned int length, FoneOSColor color);

    void SetXY(unsigned int x, unsigned int y);
    void SetOrientation(unsigned int orientation);

    void bmpdraw(std::ifstream * bmpFile, int x, int y);
};

#define ILI9341_CLOCKRATE SPI_CLOCK_DIV1

#define ILI9341_TFTWIDTH  240
#define ILI9341_TFTHEIGHT 320

#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0D
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_MADCTL  0x36
#define ILI9341_PIXFMT  0x3A

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDID4   0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1
/*
#define ILI9341_PWCTR6  0xFC

*/

class Display_ILI9341 : public Display
{
public:
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
    virtual int GetWidth();
    virtual int GetHeight();
private:
    FoneOSColor ** curBuf = NULL;
    FoneOSColor ** backBuf = NULL;

    std::map<unsigned int, mraa_gpio_context> pins;
    int _cs, _dc, _rst;
    mraa_spi_context _spi;

    void SetPin(unsigned int pin, mraa_gpio_dir_t dir);

    void DigitalWrite(unsigned int pin, unsigned int value);

    void SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

    void SPIBegin();
    void SPIEnd();

    void SPIWrite(uint8_t c);
    uint8_t SPIRead();

    uint8_t readcommand8(uint8_t c, uint8_t index = 0);

    void WriteCommand(uint8_t c);
    void WriteData(uint8_t c);

    uint16_t CreateColor(FoneOSColor color);

    void PushColor(uint16_t color);

    void DrawHorizontalLine(unsigned int x, unsigned int y, unsigned int length, FoneOSColor color);
    void DrawVerticalLine(unsigned int x, unsigned int y, unsigned int length, FoneOSColor color);

    //void SetOrientation(unsigned int orientation);

    uint8_t readByte(std::ifstream * f);
    uint16_t read16(std::ifstream * f);
    uint32_t read32(std::ifstream * f);

    bool bmpReadHeader(std::ifstream * f);

    void bmpdraw(std::ifstream * bmpFile, int x, int y);
};
#endif
